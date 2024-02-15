#pragma once
#include "DFGEngine.h"

const unsigned int NET_PACKET_INPUT_HISTORY_SIZE = 10;

struct NetworkInputPackedge
{
	uint32_t InputHistory[NET_PACKET_INPUT_HISTORY_SIZE] = { 0 };
	int FrameCount = 0;
	int FrameDelta = 0;
};

bool InitializeHost();
bool InitializeClient();

void HandleMessege();

NetworkInputPackedge ReadInputMessegeHost();
NetworkInputPackedge ReadInputMessegeClient();

void SendInputMessegeHost(NetworkInputPackedge inputPackedge);
void SendInputMessegeClient(NetworkInputPackedge inputPackedge);

NetworkInputPackedge TickNetworkHost(NetworkInputPackedge inputPackedge, bool& bRecievedInput);
NetworkInputPackedge TickNetworkClient(NetworkInputPackedge inputPackedge, bool& bRecievedInput);
