/***************************************************************************
  tag: Peter Soetens  Mon Jun 26 13:25:57 CEST 2006  ParserScriptingAccess.hpp 

                        ParserScriptingAccess.hpp -  description
                           -------------------
    begin                : Mon June 26 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@fmtc.be
 
 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/
 
 
#ifndef ORO_EXECUTION_PARSER_SCRIPTING_ACCESS_HPP
#define ORO_EXECUTION_PARSER_SCRIPTING_ACCESS_HPP

#include "ScriptingAccess.hpp"
#include "OperationInterface.hpp"

namespace RTT
{
    class StatementProcessor;

    /**
     * This implementation provides access to the loaded programs
     * and state machines and allows execution of code, using the
     * Orocos parser framework.
     */
    class ParserScriptingAccess
        : public ScriptingAccess
    {
    protected:
        StatementProcessor* sproc;
        bool doExecute(const std::string& code);

        bool doLoadPrograms( std::string filename );
        bool doLoadProgramText( std::string code );
        bool doUnloadProgram( std::string name );

        bool doLoadStateMachines( std::string filename );
        bool doLoadStateMachineText( std::string code );
        bool doUnloadStateMachine( std::string name );

        OperationInterface* createTaskObject(OperationInterface* obj);
    public:
        ParserScriptingAccess( TaskContext* parent );
        
        virtual ~ParserScriptingAccess();

        virtual int execute( const std::string& code );

        virtual CommandC getCommand( int ticket );

        virtual Functions loadFunctions(std::string filename, bool do_throw = false );
        virtual Functions loadFunctions(std::string code, std::string filename, bool do_throw = false );

        virtual bool loadPrograms( std::string filename, bool do_throw = false );
        virtual bool loadPrograms( std::string code, std::string filename, bool do_throw = false );
        virtual bool unloadProgram( std::string name, bool do_throw = false );

        virtual bool loadStateMachines( std::string filename, bool do_throw = false  );
        virtual bool loadStateMachines( std::string code, std::string filename, bool do_throw = false  );
        virtual bool unloadStateMachine( std::string name, bool do_throw = false );

    };
}

#endif
