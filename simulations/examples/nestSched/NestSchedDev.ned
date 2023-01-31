package nesting.simulations.examples.nestSched;

import nesting.faulty.node.FaultyHost;

module NestSchedDev extends FaultyHost {
	@signal[criticalFlowSig1](type=inet::Packet);
	@statistic[criticalFlowSig1Received](title="criticalFlowSig1Received"; source="dataAge(criticalFlowSig1)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig2](type=inet::Packet);
	@statistic[criticalFlowSig2Received](title="criticalFlowSig2Received"; source="dataAge(criticalFlowSig2)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig3](type=inet::Packet);
	@statistic[criticalFlowSig3Received](title="criticalFlowSig3Received"; source="dataAge(criticalFlowSig3)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig4](type=inet::Packet);
	@statistic[criticalFlowSig4Received](title="criticalFlowSig4Received"; source="dataAge(criticalFlowSig4)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig5](type=inet::Packet);
	@statistic[criticalFlowSig5Received](title="criticalFlowSig5Received"; source="dataAge(criticalFlowSig5)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig6](type=inet::Packet);
	@statistic[criticalFlowSig6Received](title="criticalFlowSig6Received"; source="dataAge(criticalFlowSig6)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig7](type=inet::Packet);
	@statistic[criticalFlowSig7Received](title="criticalFlowSig7Received"; source="dataAge(criticalFlowSig7)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig8](type=inet::Packet);
	@statistic[criticalFlowSig8Received](title="criticalFlowSig8Received"; source="dataAge(criticalFlowSig8)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig9](type=inet::Packet);
	@statistic[criticalFlowSig9Received](title="criticalFlowSig9Received"; source="dataAge(criticalFlowSig9)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[criticalFlowSig10](type=inet::Packet);
	@statistic[criticalFlowSig10Received](title="criticalFlowSig10Received"; source="dataAge(criticalFlowSig10)"; unit=s; record=histogram,vector; interpolationmode=linear);

	@signal[noncriticalFlowSig1](type=inet::Packet);
	@statistic[noncriticalFlowSig1Received](title="noncriticalFlowSig1Received"; source="dataAge(noncriticalFlowSig1)"; unit=s; record=histogram,vector; interpolationmode=linear);
	@signal[noncriticalFlowSig2](type=inet::Packet);
	@statistic[noncriticalFlowSig2Received](title="noncriticalFlowSig2Received"; source="dataAge(noncriticalFlowSig2)"; unit=s; record=histogram,vector; interpolationmode=linear);
	@signal[noncriticalFlowSig3](type=inet::Packet);
	@statistic[noncriticalFlowSig3Received](title="noncriticalFlowSig3Received"; source="dataAge(noncriticalFlowSig3)"; unit=s; record=histogram,vector; interpolationmode=linear);
	@signal[noncriticalFlowSig4](type=inet::Packet);
	@statistic[noncriticalFlowSig4Received](title="noncriticalFlowSig4Received"; source="dataAge(noncriticalFlowSig4)"; unit=s; record=histogram,vector; interpolationmode=linear);
	@signal[noncriticalFlowSig5](type=inet::Packet);
	@statistic[noncriticalFlowSig5Received](title="noncriticalFlowSig5Received"; source="dataAge(noncriticalFlowSig5)"; unit=s; record=histogram,vector; interpolationmode=linear);
	@signal[noncriticalFlowSig6](type=inet::Packet);
	@statistic[noncriticalFlowSig6Received](title="noncriticalFlowSig6Received"; source="dataAge(noncriticalFlowSig6)"; unit=s; record=histogram,vector; interpolationmode=linear);

}