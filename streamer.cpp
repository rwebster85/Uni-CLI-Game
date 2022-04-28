#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "streamer.h"

using namespace raw;

using boost::asio::ip::tcp;

Streamer* Streamer::instance = 0;

Streamer::Streamer()
{
	SetupConnection();
}

void Streamer::SetupConnection()
{
	boost::asio::io_service ios;

	tcp::endpoint endpoint(tcp::v4(), 8000);
	tcp::acceptor acceptor(ios, endpoint);

	acceptor.accept(*Stream.rdbuf());
}

Streamer* Streamer::GetStreamer()
{
	if (instance == 0)
	{
		instance = new Streamer();
	}

	return instance;
}

tcp::iostream* Streamer::GetStream()
{
	return &Stream;
}
