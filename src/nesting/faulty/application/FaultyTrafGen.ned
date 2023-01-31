package nesting.faulty.application;

import nesting.application.ethernet.VlanEtherTrafGenSched;

simple FaultyTrafGen extends VlanEtherTrafGenSched
{
    parameters:
        @class(FaultyTrafGen);
        int dropAtStartProb = default(0);
        int dropAtEndProb = default(0);
}