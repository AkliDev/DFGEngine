
#include "Network.h"

#define NBNET_IMPL
#include "nbnet.h"
#include "net_drivers/udp.h"

#define HOST_PORT 50101
#define PROTOCOL_NAME "clock_sync_test"
#define ECHO_SERVER_BUSY_CODE 42

static NBN_Connection* client = nullptr;

bool InitializeHost()
{
#ifdef __EMSCRIPTEN__
	NBN_WebRTC_Register(); // Register the WebRTC driver
#else
	NBN_UDP_Register(); // Register the UDP driver
#endif // __EMSCRIPTEN__ 

	NBN_GameServer_Init(PROTOCOL_NAME, HOST_PORT, false);
	if (NBN_GameServer_Start() < 0)
	{
		DFG_ERROR("Failed to start the server");
		NBN_GameServer_Stop();

#ifdef __EMSCRIPTEN__
		emscripten_force_exit(1);
#else
		return true;
#endif
	}
	DFG_INFO("Started the hosting server");
	return false;
}

bool InitializeClient()
{
#ifdef __EMSCRIPTEN__
	NBN_WebRTC_Register(); // Register the WebRTC driver
#else
	NBN_UDP_Register(); // Register the UDP driver
#endif // __EMSCRIPTEN__

	// Start the client with a protocol name (must be the same than the one used by the server)
	// the server host and port
	NBN_GameClient_Init(PROTOCOL_NAME, "127.0.0.1", HOST_PORT,false, NULL);
	if (NBN_GameClient_Start() < 0)
	{
		DFG_ERROR("Failed to start client");

		// Error, quit the client application
#ifdef __EMSCRIPTEN__
		emscripten_force_exit(1);
#else
		return true;
#endif 
	}
	DFG_INFO("Started the client");
	return false;
}

void HandleMessege()
{
	// Get info about the received message
	NBN_MessageInfo msg_info = NBN_GameServer_GetMessageInfo();

	assert(msg_info.sender == client);
	assert(msg_info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE);

	// Retrieve the received message
	NBN_ByteArrayMessage* msg = (NBN_ByteArrayMessage*)msg_info.data;

	// Destroy the received message
	NBN_ByteArrayMessage_Destroy(msg);
}

NetworkInputPackedge ReadInputMessegeCommon(const NBN_MessageInfo messegeInfo)
{
	assert(messegeInfo.type == NBN_BYTE_ARRAY_MESSAGE_TYPE);

	// Retrieve the received message
	NBN_ByteArrayMessage* msg = (NBN_ByteArrayMessage*)messegeInfo.data;

	NetworkInputPackedge Packedge;
	memcpy(&Packedge, msg->bytes, sizeof(Packedge));

	// Destroy the received message
	NBN_ByteArrayMessage_Destroy(msg);

	return Packedge;
}

NetworkInputPackedge ReadInputMessegeHost()
{
	// Get info about the received message
	NBN_MessageInfo msg_info = NBN_GameServer_GetMessageInfo();

	//assert(msg_info.sender == client);

	return ReadInputMessegeCommon(msg_info);
}

NetworkInputPackedge ReadInputMessegeClient()
{
	// Get info about the received message
	NBN_MessageInfo msg_info = NBN_GameClient_GetMessageInfo();

	return ReadInputMessegeCommon(msg_info);
}

void SendInputMessegeHost(NetworkInputPackedge inputPackedge)
{
	unsigned int length = sizeof(inputPackedge); // Compute message length

	if (NBN_GameServer_SendUnreliableByteArrayTo(client, (uint8_t*)&inputPackedge, length) < 0)
	{
		DFG_ERROR("Failed to send outgoing messege");
		exit(-1);
	}
}

void SendInputMessegeClient(NetworkInputPackedge inputPackedge)
{
	unsigned int length = sizeof(inputPackedge); // Compute message length

	if (NBN_GameClient_SendUnreliableByteArray((uint8_t*)&inputPackedge, length) < 0)
	{
		DFG_ERROR("Failed to send outgoing messege");
		exit(-1);
	}
}

NetworkInputPackedge TickNetworkHost(NetworkInputPackedge inputPackedge, bool& bRecievedInput)
{
	bRecievedInput = false;
	if (client)
	{
		for (int i = 0; i < 5; i++)
		{
			SendInputMessegeHost(inputPackedge);
		}
	}

	NetworkInputPackedge clientInputPackedge{0, -1};
	// Update the server clock
	NBN_GameServer_AddTime(1.0/60.0);

	int ev;

	// Poll for server events
	while ((ev = NBN_GameServer_Poll()) != NBN_NO_EVENT)
	{
		if (ev < 0)
		{
			DFG_ERROR("Something went wrong");

			// Error, quit the server application
			exit(-1);
			break;
		}

		switch (ev)
		{
			// New connection request...
		case NBN_NEW_CONNECTION:
			// Echo server work with one single client at a time
			if (client == nullptr)
			{
				client = NBN_GameServer_GetIncomingConnection();
				NBN_GameServer_AcceptIncomingConnection();
				DFG_INFO("Accepting client connection!");
			}

			break;

		//	// The client has disconnected
		//case NBN_CLIENT_DISCONNECTED:
		//	assert(NBN_GameServer_GetDisconnectedClient()->id == client->id);
		//
		//	client = NULL;
		//	break;
		//
		//	// A message has been received from the client
		case NBN_CLIENT_MESSAGE_RECEIVED:
			clientInputPackedge = ReadInputMessegeHost();
			bRecievedInput = true;
			break;
		}
	}

	// Pack all enqueued messages as packets and send them
	if (NBN_GameServer_SendPackets() < 0)
	{
		DFG_ERROR("Failed to send packets");

		// Error, quit the server application
		exit(-1);
	}

	return clientInputPackedge;
}

NetworkInputPackedge TickNetworkClient(NetworkInputPackedge inputPackedge, bool& bRecievedInput)
{
	bRecievedInput = false;
	NetworkInputPackedge hostInputPackedge{ 0, -1 };

	for (int i = 0; i < 5; i++)
	{
		SendInputMessegeClient(inputPackedge);
	}
	
	// Update client clock
	NBN_GameClient_AddTime(1.0/60);

	int ev;

	// Poll for client events
	while ((ev = NBN_GameClient_Poll()) != NBN_NO_EVENT)
	{
		if (ev < 0)
		{
			DFG_ERROR("An error occured while polling client events. Exit");

			exit(-1);
		}

		switch (ev)
		{
		// Client is connected to the server
		case NBN_CONNECTED:
			//OnConnected();
			DFG_TRACE("Connected to host!");
			break;

		//	// Client has disconnected from the server
		//case NBN_DISCONNECTED:
		//	//OnDisconnected();
		//	DFG_TRACE("Disconnected from host!");
		//	break;
		//
			// A message has been received from the server
		case NBN_MESSAGE_RECEIVED:
			hostInputPackedge = ReadInputMessegeClient();
			bRecievedInput = true;
			break;
		}
	}

	// Pack all enqueued messages as packets and send them
	if (NBN_GameClient_SendPackets() < 0)
	{
		DFG_ERROR("Failed to send packets.Exit");

		// Stop main loop
		exit(-1);
	}
	return hostInputPackedge;
}