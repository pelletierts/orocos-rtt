/***************************************************************************
  tag: The SourceWorks  Tue Sep 7 00:55:18 CEST 2010  ServiceRequesterI.cpp

                        ServiceRequesterI.cpp -  description
                           -------------------
    begin                : Tue September 07 2010
    copyright            : (C) 2010 The SourceWorks
    email                : peter@thesourceworks.com

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public                   *
 *   License as published by the Free Software Foundation;                 *
 *   version 2 of the License.                                             *
 *                                                                         *
 *   As a special exception, you may use this file as part of a free       *
 *   software library without restriction.  Specifically, if other files   *
 *   instantiate templates or use macros or inline functions from this     *
 *   file, or you compile this file and link it with other files to        *
 *   produce an executable, this file does not by itself cause the         *
 *   resulting executable to be covered by the GNU General Public          *
 *   License.  This exception does not however invalidate any other        *
 *   reasons why the executable file might be covered by the GNU General   *
 *   Public License.                                                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public             *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/


// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// ../../../ACE_wrappers/TAO/TAO_IDL/be/be_codegen.cpp:1196

#include "ServiceRequesterI.h"
#include "CorbaOperationCallerFactory.hpp"
#include "TaskContext.hpp"

using namespace RTT;
using namespace RTT::detail;

// Implementation skeleton constructor
RTT_corba_CServiceRequester_i::RTT_corba_CServiceRequester_i ( RTT::ServiceRequester* service, PortableServer::POA_ptr poa )
    : mservice(service), mpoa( PortableServer::POA::_duplicate(poa) )
{
}

// Implementation skeleton destructor
RTT_corba_CServiceRequester_i::~RTT_corba_CServiceRequester_i (void)
{
}

char * RTT_corba_CServiceRequester_i::getRequestName (
    void)
{
    return CORBA::string_dup( mservice->getRequestName().c_str() );
}

::RTT::corba::CRequestNames * RTT_corba_CServiceRequester_i::getRequestNames (
    void)
{
    ServiceRequester::RequesterNames names = mservice->getRequesterNames();
    ::RTT::corba::CRequestNames_var result = new ::RTT::corba::CRequestNames();
    result->length( names.size() );
    for (unsigned int i=0; i != names.size(); ++i )
        result[i] = CORBA::string_dup( names[i].c_str() );

    return result._retn();
}

::RTT::corba::COperationCallerNames * RTT_corba_CServiceRequester_i::getOperationCallerNames (
    void)
{
    ServiceRequester::OperationCallerNames names = mservice->getOperationCallerNames();
    ::RTT::corba::COperationCallerNames_var result = new ::RTT::corba::COperationCallerNames();
    result->length( names.size() );
    for (unsigned int i=0; i != names.size(); ++i )
        result[i] = CORBA::string_dup( names[i].c_str() );

    return result._retn();
}

::RTT::corba::CServiceRequester_ptr RTT_corba_CServiceRequester_i::getRequest (
    const char * service_name)
{
    string svc(service_name);
    if ( svc == "this" )
        return _this();

    if ( mservice->requiresService(service_name) == false )
        return RTT::corba::CServiceRequester::_nil();

    // Creates service requester
    if ( mrequests.find(svc) == mrequests.end() ) {
        log(Debug) << "Creating CServiceRequester for "<< service_name <<endlog();
        RTT_corba_CServiceRequester_i* mserv;
        mserv = new RTT_corba_CServiceRequester_i( mservice->requires(service_name), mpoa );
        CServiceRequester_ptr request = mserv->activate_this();
        mrequests[svc] = pair<RTT::corba::CServiceRequester_var, PortableServer::ServantBase_var>(request,mserv);
    }
    // Now return it.
    return RTT::corba::CServiceRequester::_duplicate( mrequests[svc].first.in() );
}

::CORBA::Boolean RTT_corba_CServiceRequester_i::hasRequest (
    const char * name)
{
    return mservice->requiresService( name );
}

::CORBA::Boolean RTT_corba_CServiceRequester_i::connectTo (
    ::RTT::corba::CService_ptr svc)
{
    ServiceRequester::OperationCallerNames names = mservice->getOperationCallerNames();

    for (unsigned int i=0; i != names.size(); ++i )
        connectCallerTo( names[i].c_str(), svc);

    return mservice->ready();;
}

::CORBA::Boolean RTT_corba_CServiceRequester_i::connectCallerTo (
      const char * name,
      ::RTT::corba::CService_ptr svc)
{
    string oname(name);
    if ( mservice->getOperationCaller(oname) == 0) {
        log(Error) << "No such OperationCaller: " << oname << " in "<< mservice->getRequestName()<<endlog();
        return false;
    }
    if ( svc->getArity(oname.c_str()) == -1) {
        CORBA::String_var svcname = svc->getName();
        log(Error) << "No such Operation: " << oname << " in "<< svcname.in() << endlog();
        return false;
    }

    // creates a local factory for this remote method.
    OperationInterfacePart* tmp = new CorbaOperationCallerFactory(oname,svc, mpoa.in() );
    return mservice->getOperationCaller(oname)->setImplementationPart( tmp, mservice->getServiceOwner()->engine() );
}

::CORBA::Boolean RTT_corba_CServiceRequester_i::disconnectCaller (
      const char * name )
{
    string oname(name);
    if ( mservice->getOperationCaller(oname) == 0) {
        log(Error) << "No such OperationCaller: " << oname << " in "<< mservice->getRequestName()<<endlog();
        return false;
    }
    // disconnects.
    mservice->getOperationCaller(oname)->disconnect();
    return true;
}

::CORBA::Boolean RTT_corba_CServiceRequester_i::callerReady (
      const char * name )
{
    string oname(name);
    if ( mservice->getOperationCaller(oname) == 0) {
        log(Error) << "No such OperationCaller: " << oname << " in "<< mservice->getRequestName()<<endlog();
        return false;
    }
    // disconnects.
    return mservice->getOperationCaller(oname)->ready();
}

::CORBA::Boolean RTT_corba_CServiceRequester_i::ready (
    void)
{
    return mservice->ready();
}

void RTT_corba_CServiceRequester_i::disconnect (
    void)
{
    mservice->disconnect();
}


