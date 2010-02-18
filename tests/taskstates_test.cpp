/***************************************************************************
  tag: Peter Soetens  Mon Jun 26 13:26:02 CEST 2006  generictask_test.cpp

                        generictask_test_3.cpp -  description
                           -------------------
    begin                : Mon June 26 2006
    copyright            : (C) 2006 Peter Soetens
    email                : peter.soetens@fmtc.be

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



#include <iostream>

#include <TaskContext.hpp>
#include <extras/SlaveActivity.hpp>
#include <extras/SequentialActivity.hpp>
#include <extras/SimulationActivity.hpp>
#include <extras/SimulationThread.hpp>

#include <boost/function_types/function_type.hpp>
#include <Method.hpp>

using namespace std;
using namespace RTT;
using namespace RTT::detail;


#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

struct A {};


// Test TaskContext states.
class StatesTC
    : public RTT::TaskContext
{
public:
    StatesTC()
        : TaskContext("TC", PreOperational)
    {
        BOOST_CHECK( this->getTaskState() == TaskContext::PreOperational );
        this->resetFlags();
        validconfig = true;
        validstart = true;
        validreset = true;
        do_error = false;
    }

    void resetFlags()
    {
        didconfig = false;
        didstart = false;
        didstop = false;
        didcleanup = false;
        didupdate = false;
        didreset = false;
    }

    bool configureHook() {
        BOOST_CHECK( mTaskState <= Stopped );
        didconfig = true;
        return validconfig;
    }

    bool startHook() {
        BOOST_CHECK( mTaskState == Stopped);
        didstart = true;
        return validstart;
    }

    void stopHook() {
        BOOST_CHECK( mTaskState >= Running || mTaskState == FatalError);
        didstop = true;
    }

    void cleanupHook() {
        BOOST_CHECK( mTaskState == Stopped );
        didcleanup = true;
    }

    void updateHook() {
        BOOST_CHECK( mTaskState == Running );
        didupdate = true;
        if (do_error)
            this->fatal();
        if (do_throw)
            throw A();
    }

    bool resetHook() {
        didreset = true;
        return validreset;
    }

    bool validconfig, didconfig;
    bool validstart, didstart;
    bool didstop;
    bool didcleanup;
    bool didupdate;
    bool validreset, didreset;
    bool do_error, do_throw;
};

/**
 * Fixture.
 */
class TaskStates_Test
{
public:
    TaskContext* tc;
    StatesTC* stc;
    ActivityInterface* tsim;
    ActivityInterface* stsim;

public:
    TaskStates_Test()
    {
        tc =  new TaskContext( "root", TaskContext::Stopped );
        stc = new StatesTC();
        tc->setActivity( new SimulationActivity(0.001) );
        stc->setActivity( new SimulationActivity(0.001) );
        tsim = tc->getActivity();
        stsim = tc->getActivity();
        SimulationThread::Instance()->stop();
    }

    ~TaskStates_Test()
    {
        tsim->stop();
        stsim->stop();
        delete tc;
        delete stc;
    }
};


// Registers the fixture into the 'registry'
BOOST_FIXTURE_TEST_SUITE(  TaskStatesSuite,  TaskStates_Test )


/**
 * Tests the initial period of a TC after setting an activity.
 */
BOOST_AUTO_TEST_CASE( testPeriod)
{
    // check unconfigured TC
    TaskContext pertc("PerTC");
    BOOST_CHECK( pertc.getPeriod() == 0.0 );
    BOOST_CHECK( pertc.isActive() );

    // check periodic TC
    pertc.setActivity( new SlaveActivity(1.0) );
    BOOST_CHECK( pertc.engine()->getActivity()->getPeriod() == 1.0 );
    BOOST_CHECK( pertc.getPeriod() == 1.0 );

    // check non periodic TC
    pertc.setActivity( new SlaveActivity(0.0) );
    BOOST_CHECK( pertc.engine()->getActivity()->getPeriod() == 0.0 );
    BOOST_CHECK( pertc.getPeriod() == 0.0 );
}

/**
 * Tests the normal TC states.
 */
BOOST_AUTO_TEST_CASE( testTCStates)
{
    // Test the states of a Default TC, no user methods.
    BOOST_CHECK( tc->getTaskState() == TaskContext::Stopped );

    // Configure in Stop
    BOOST_CHECK( tc->isConfigured() == true );
    BOOST_CHECK( tc->isRunning() == false );
    BOOST_CHECK( tc->isActive() == true );
    BOOST_CHECK( tc->configure() == true );
    BOOST_CHECK( tc->resetError() == false );

    // Stop to Running
    BOOST_CHECK( tc->start() == true );
    BOOST_CHECK( tc->isRunning() == true );
    BOOST_CHECK( tc->isActive() == true );
    BOOST_CHECK( tc->configure() == false );
    BOOST_CHECK( tc->start() == false );
    BOOST_CHECK( tc->cleanup() == false );
    BOOST_CHECK( tc->resetError() == false );

    // Running to Stop
    BOOST_CHECK( tc->stop() == true );
    BOOST_CHECK( tc->isRunning() == false);
    BOOST_CHECK( tc->isActive() == true );
    BOOST_CHECK( tc->stop() == false );
    BOOST_CHECK( tc->configure() == true );
    BOOST_CHECK( tc->isConfigured() == true );

    // Engine Stop to Active to Stop
    BOOST_CHECK( tc->engine()->getActivity()->stop() == true);
    BOOST_CHECK( tc->isActive() == false);
    BOOST_CHECK( tc->activate() == true );
    BOOST_CHECK( tc->isActive() == true);
    BOOST_CHECK( tc->isRunning() == false );
    BOOST_CHECK( tc->configure() == true );
    BOOST_CHECK( tc->resetError() == false );
    BOOST_CHECK( tc->isConfigured() == true );

    // Stop to PreOp
    BOOST_CHECK( tc->cleanup() == true );
    BOOST_CHECK( tc->isConfigured() == false);
    BOOST_CHECK( tc->isRunning() == false);

    // PreOp to stop
    BOOST_CHECK( tc->configure() == true );
    BOOST_CHECK( tc->isConfigured() == true);
    BOOST_CHECK( tc->isRunning() == false);
    BOOST_CHECK( tc->isActive() == true );

}

/**
 * Tests mainly the error states.
 */
BOOST_AUTO_TEST_CASE( testSpecialTCStates)
{
    // Test the states of a Specially crafted TC, requiring configure etc.
    BOOST_CHECK( stc->getTaskState() == TaskContext::PreOperational );

    // PreOperational state:
    BOOST_CHECK( stc->isConfigured() == false );
    BOOST_CHECK( stc->isRunning() == false );
    BOOST_CHECK( stc->configure() == true );
    BOOST_CHECK( stc->didconfig == true );
    BOOST_CHECK( stc->isConfigured() == true );
    // test flags
    BOOST_CHECK( stc->didstart == false );
    BOOST_CHECK( stc->didupdate == false );
    BOOST_CHECK( stc->didstop == false );
    BOOST_CHECK( stc->didcleanup == false );
    stc->resetFlags();

    // Stopped state:
    BOOST_CHECK( stc->start() == true );
    BOOST_CHECK( stc->didstart == true );
    stc->resetFlags();
    BOOST_CHECK( stc->isRunning() == true );
    BOOST_CHECK( stc->configure() == false );
    BOOST_CHECK( stc->start() == false );
    BOOST_CHECK( stc->cleanup() == false );
    // test flags
    BOOST_CHECK( stc->didconfig == false );
    BOOST_CHECK( stc->didstart == false );
    BOOST_CHECK( stc->didupdate == false );
    BOOST_CHECK( stc->didstop == false );
    BOOST_CHECK( stc->didcleanup == false );
    stc->resetFlags();


    // Running state / updateHook :
    SimulationThread::Instance()->run(1);
    // test flags
    BOOST_CHECK( stc->didconfig == false );
    BOOST_CHECK( stc->didstart == false );
    BOOST_CHECK( stc->didupdate == true );
    BOOST_CHECK( stc->didstop == false );
    BOOST_CHECK( stc->didcleanup == false );
    stc->resetFlags();

    // Back to stopped
    BOOST_CHECK( stc->stop() == true );
    BOOST_CHECK( stc->didstop == true );
    // test flags
    BOOST_CHECK( stc->didconfig == false );
    BOOST_CHECK( stc->didstart == false );
    BOOST_CHECK( stc->didupdate == false );
    BOOST_CHECK( stc->didstop == true );
    BOOST_CHECK( stc->didcleanup == false );
    stc->resetFlags();

    BOOST_CHECK( stc->isRunning() == false);
    BOOST_CHECK( stc->stop() == false );
    BOOST_CHECK( stc->configure() == true );
    BOOST_CHECK( stc->isConfigured() == true );
    // test flags
    BOOST_CHECK( stc->didconfig == true );
    BOOST_CHECK( stc->didstart == false );
    BOOST_CHECK( stc->didupdate == false );
    BOOST_CHECK( stc->didstop == false );
    BOOST_CHECK( stc->didcleanup == false );
    stc->resetFlags();

    // Stopped to PreOp state:
    BOOST_CHECK( stc->cleanup() == true );
    BOOST_CHECK( stc->isConfigured() == false);
    BOOST_CHECK( stc->isRunning() == false);
    // test flags
    BOOST_CHECK( stc->didconfig == false );
    BOOST_CHECK( stc->didstart == false );
    BOOST_CHECK( stc->didupdate == false );
    BOOST_CHECK( stc->didstop == false );
    BOOST_CHECK( stc->didcleanup == true );
    stc->resetFlags();
    BOOST_CHECK( stc->start() == false );
    BOOST_CHECK( stc->cleanup() == false );
    // test flags
    BOOST_CHECK( stc->didconfig == false );
    BOOST_CHECK( stc->didstart == false );
    BOOST_CHECK( stc->didupdate == false );
    BOOST_CHECK( stc->didstop == false );
    BOOST_CHECK( stc->didcleanup == false );
    stc->resetFlags();


    // PreOperational to Stopped state:
    BOOST_CHECK( stc->configure() == true );
    BOOST_CHECK( stc->isConfigured() == true);
    BOOST_CHECK( stc->isRunning() == false);
    // test flags
    BOOST_CHECK( stc->didconfig == true );
    BOOST_CHECK( stc->didstart == false );
    BOOST_CHECK( stc->didupdate == false );
    BOOST_CHECK( stc->didstop == false );
    BOOST_CHECK( stc->didcleanup == false );
    stc->resetFlags();

}

BOOST_AUTO_TEST_CASE( testFailingTCStates)
{
    // Test the states of a TC failing in transitions
    stc->validconfig = false;
    stc->validstart = false;
    stc->validreset = false;

    // PreOperational state:
    BOOST_CHECK( stc->isConfigured() == false );
    BOOST_CHECK( stc->isRunning() == false );
    BOOST_CHECK( stc->configure() == false );
    BOOST_CHECK( stc->didconfig == true );
    BOOST_CHECK( stc->isConfigured() == false );
    BOOST_CHECK( stc->isRunning() == false );
    BOOST_CHECK( stc->isActive() == true );
    stc->resetFlags();

    // Retry:
    stc->validconfig = true;
    BOOST_CHECK( stc->configure() == true );
    BOOST_CHECK( stc->didconfig == true );
    BOOST_CHECK( stc->isConfigured() == true );
    BOOST_CHECK( stc->isRunning() == false );
    BOOST_CHECK( stc->isActive() == true );
    stc->resetFlags();

    // Stopped state:
    BOOST_CHECK( stc->start() == false );
    BOOST_CHECK( stc->didstart == true );
    BOOST_CHECK( stc->isRunning() == false );
    BOOST_CHECK( stc->isActive() == true );
    BOOST_CHECK( stc->isConfigured() == true );
    stc->resetFlags();

    // Retry:
    stc->validstart = true;
    BOOST_CHECK( stc->start() == true );
    BOOST_CHECK( stc->didstart == true );
    BOOST_CHECK( stc->isConfigured() == true );
    BOOST_CHECK( stc->isRunning() == true );
    BOOST_CHECK( stc->isActive() == true );
    stc->resetFlags();

    // Error state.
    stc->do_error = true;
    // Running state / updateHook :
    SimulationThread::Instance()->run(1);
    BOOST_CHECK( stc->inFatalError() == true );
    BOOST_CHECK( stc->isActive() == false );  // no longer active
    BOOST_CHECK( stc->activate() == true );
    BOOST_CHECK( stc->resetError() == true ); // brings us to PreOp.
    BOOST_CHECK( stc->didreset == true );
    BOOST_CHECK( stc->isConfigured() == false);
    // Retry:
    BOOST_CHECK( stc->configure() == true);
    BOOST_CHECK( stc->start() == true );
    stc->validreset = true;
    stc->didreset   = false;
    // Running state / updateHook :
    SimulationThread::Instance()->run(1);
    BOOST_CHECK( stc->inFatalError() == true );
    BOOST_CHECK( stc->resetError() == true ); // brings us to Stopped.
    BOOST_CHECK( stc->didreset == true );
    BOOST_CHECK( stc->isConfigured() == true);

}

BOOST_AUTO_TEST_CASE( testExecutionEngine)
{
    // no owner:
    ExecutionEngine ee1(0);
    ExecutionEngine ee2(0);

    // test setActivity:
    BOOST_CHECK( tsim->stop() );
    BOOST_CHECK( tsim->run(&ee1) );
    BOOST_CHECK(tsim->start() );
    BOOST_CHECK( SimulationThread::Instance()->run(5) );

    // this also tests setActivity:
    BOOST_CHECK( tsim->stop() );
    BOOST_CHECK( tsim->run(&ee2) );
    BOOST_CHECK(tsim->start() );
    BOOST_CHECK( SimulationThread::Instance()->run(5) );

    {
        TaskCore tc1(&ee2);
        TaskCore tc2(&ee2);

        // run with two children.
        BOOST_CHECK( SimulationThread::Instance()->run(5) );
    }
    // children removed again:
    BOOST_CHECK( SimulationThread::Instance()->run(5) );
    BOOST_CHECK(tsim->stop() );
    tsim->run(0);
}

BOOST_AUTO_TEST_SUITE_END()

