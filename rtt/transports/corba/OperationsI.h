/***************************************************************************
  tag: Peter Soetens  Mon Jun 26 13:25:58 CEST 2006  ExecutionI.h

                        ExecutionI.h -  description
                           -------------------
    begin                : Mon June 26 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@fmtc.be

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
// be/be_codegen.cpp:859

// must be outside of #ifdef macro.
#include "corba.h"
#include "../../internal/DataSource.hpp"

#ifndef INCLUDE_EXECUTIONI_H_
#define INCLUDE_EXECUTIONI_H_

#include "OperationsC.h"
#ifdef CORBA_IS_TAO
#include "OperationsS.h"
#endif
#include "CorbaConversion.hpp"
#include "AnyDataSource.hpp"
#include "../../base/DispatchInterface.hpp"
#include "../../Logger.hpp"
#include "../../internal/MethodC.hpp"
#include "../../internal/DataSources.hpp"
#include "CorbaTypeTransporter.hpp"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

using namespace RTT;

/**
 * A servant which serves a RTT::internal::DataSource through the 'Any' methods.
 */
class  Orocos_AnyExpression_i
  : public virtual POA_RTT::corba::CExpression,
    public virtual PortableServer::RefCountServantBase
{
protected:
    RTT::base::DataSourceBase::shared_ptr morig;
    CORBA::Any_var last_value;
    PortableServer::POA_var mpoa;
    corba::CorbaTypeTransporter* ctp;
public:
    typedef RTT::base::DataSourceBase::const_ptr SourceType;
    typedef CORBA::Any                             ResultType;

    virtual void copy( RTT::base::DataSourceBase::shared_ptr new_ds ) {
        morig = new_ds;
    }

  // Constructor
  Orocos_AnyExpression_i (RTT::base::DataSourceBase::shared_ptr orig, PortableServer::POA_ptr the_poa)
      : morig( orig ), last_value( 0 )
        , mpoa( PortableServer::POA::_duplicate(the_poa) )
    {
      types::TypeTransporter* tp = morig->getTypeInfo()->getProtocol(ORO_CORBA_PROTOCOL_ID);
      ctp = dynamic_cast<corba::CorbaTypeTransporter*>(tp);
      assert(ctp);
      last_value = ctp->createAny( morig );
    }

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }

    virtual RTT::corba::CExpression_ptr activate_this() {
        PortableServer::ObjectId_var oid = mpoa->activate_object(this); // ref count=2
        _remove_ref(); // ref count=1
        return _this();
    }

  // Destructor
    virtual ~Orocos_AnyExpression_i (void) {}

  virtual
  CORBA::Any* value (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return new CORBA::Any( last_value.in() );
  }

  virtual
  CORBA::Boolean evaluate (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      morig->evaluate();
      last_value = ctp->createAny(morig);
      bool result = true;
      // if it is a bool, update result and return it, otherwise, just return true:
      RTT::corba::AnyConversion<bool>::update( last_value.in(), result );
      return result;
  }

  virtual
  CORBA::Any * get (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      morig->evaluate();
      return ctp->createAny(morig);
  }

  virtual
  char * getType (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getType().c_str() );
  }

  virtual
  char * toString (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      std::string result = morig->toString();
      return CORBA::string_dup( result.c_str() );
  }


  virtual
  char * getTypeName (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return CORBA::string_dup( morig->getTypeName().c_str() );
  }

  virtual void destroyExpression()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};


class  Orocos_AnyAssignableExpression_i
    : public Orocos_AnyExpression_i,
      public virtual POA_RTT::corba::CAssignableExpression
{
    RTT::base::DataSourceBase::shared_ptr mset;
    corba::CorbaTypeTransporter* ctp;
public:

    virtual void copy( RTT::base::DataSourceBase::shared_ptr new_ds ) {
        mset = new_ds;
        morig = new_ds;
    }

  // Constructor
  Orocos_AnyAssignableExpression_i (RTT::base::DataSourceBase::shared_ptr orig, PortableServer::POA_ptr the_poa)
      : Orocos_AnyExpression_i(orig, the_poa), mset( orig )
   {
      types::TypeTransporter* tp = mset->getTypeInfo()->getProtocol(ORO_CORBA_PROTOCOL_ID);
      ctp = dynamic_cast<corba::CorbaTypeTransporter*>(tp);
      assert(ctp);
   }

  // Destructor
    virtual ~Orocos_AnyAssignableExpression_i (void) {}

    virtual RTT::corba::CAssignableExpression_ptr activate_this() {
        PortableServer::ObjectId_var oid = mpoa->activate_object(this); // ref count=2
        _remove_ref(); // ref count=1
        return _this();
    }

  virtual
  CORBA::Boolean set (
      const ::CORBA::Any & value
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return ctp->updateFromAny(&value, mset );
  }

  virtual
  CORBA::Boolean fromString (
      const char* value
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {

      if ( !mset->getTypeInfo()->fromString( value, mset ) ) {
          RTT::log(RTT::Error) << "corba::CAssignableExpression: Could not assign string to "<<mset->getType() <<"." <<RTT::endlog()
			       <<" Tried to assign as "<< RTT::internal::DataSource<ResultType>::GetType() << " to native type "<< RTT::internal::DataSource<SourceType>::GetType()<< RTT::endlog();
          return false;
      }
      return true;
  }

};


class  Orocos_CAction_i
    : public virtual POA_RTT::corba::CAction,
      public virtual PortableServer::RefCountServantBase
{
	RTT::internal::MethodC morig;
	RTT::base::ActionInterface* mcom;
    PortableServer::POA_var mpoa;
public:
  //Constructor
  Orocos_CAction_i ( RTT::internal::MethodC* orig, RTT::base::ActionInterface* com, PortableServer::POA_ptr the_poa );

    PortableServer::POA_ptr _default_POA()
    {
        return PortableServer::POA::_duplicate(mpoa);
    }

  //Destructor
    virtual ~Orocos_CAction_i (void);

    virtual RTT::corba::CAction_ptr activate_this() {
        PortableServer::ObjectId_var oid = mpoa->activate_object(this); // ref count=2
        _remove_ref(); // ref count=1
        return _this();
    }

    virtual
  CORBA::Boolean execute (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ));

  virtual
  CORBA::Boolean executeAny (
      const ::RTT::corba::CAnyArguments& args
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ,::RTT::corba::CWrongNumbArgException
    ,::RTT::corba::CWrongTypeArgException
    ));

  virtual
  void reset (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      ));

  virtual void destroyAction()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};

class  Orocos_AnyMethod_i
    : public virtual Orocos_AnyExpression_i,
      public virtual POA_RTT::corba::CMethod
{
protected:
	RTT::internal::MethodC mmethodc;

public:
  virtual void copy( RTT::base::DataSourceBase::shared_ptr new_ds ) {
        morig = new_ds;
    }

  //Constructor
  Orocos_AnyMethod_i (RTT::internal::MethodC orig, RTT::base::DataSourceBase::shared_ptr datas, PortableServer::POA_ptr the_poa )
      : Orocos_AnyExpression_i( datas, the_poa ), mmethodc(orig)
    {}

  //Destructor
    virtual ~Orocos_AnyMethod_i (void) {}

    virtual RTT::corba::CMethod_ptr activate_this() {
        PortableServer::ObjectId_var oid = mpoa->activate_object(this); // ref count=2
        _remove_ref(); // ref count=1
        return _this();
    }
    virtual
  CORBA::Boolean execute (

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      return this->evaluate();
  }

  CORBA::Boolean executeAny (
      const ::RTT::corba::CAnyArguments& args
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ,::RTT::corba::CWrongNumbArgException
    ,::RTT::corba::CWrongTypeArgException
	  )) {
      RTT::internal::MethodC mgen = mmethodc;
    try {
        for (size_t i =0; i != args.length(); ++i)
	  mgen.arg( RTT::base::DataSourceBase::shared_ptr( new RTT::corba::AnyDataSource( new CORBA::Any( args[i] ) )));
        // if not ready, not enough args were given, *guess* a one off error in the exception :-(
        if ( !mgen.ready() )
            throw ::RTT::corba::CWrongNumbArgException( args.length()+1, args.length() );
        morig = mgen.getDataSource();
        return this->evaluate();
    } catch ( RTT::internal::wrong_number_of_args_exception& wna ) {
        throw ::RTT::corba::CWrongNumbArgException( wna.wanted, wna.received );
    } catch ( RTT::internal::wrong_types_of_args_exception& wta ) {
        throw ::RTT::corba::CWrongTypeArgException( wta.whicharg, wta.expected_.c_str(), wta.received_.c_str() );
    }
    return false;
  }

  virtual
  void reset(

    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      )) {
      this->morig->reset();
  }

  virtual void destroyAction()
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
 {
  }

};

#endif /* INCLUDE_EXPRESSIONI_H_  */
