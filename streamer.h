#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace raw {
	/**
	 * Handles sending/receiving data across the network using boost.
	 * Singleton pattern.
	 */
	class Streamer {
	private:
		 /** Stored single instance of this class. */
		static Streamer* instance;

		/** The stored network stream. */
		tcp::iostream Stream;

		/**
		 * Private constructor. Singleton.
		 * Called by GetStreamer(). Calls SetupConnection().
		 */
		Streamer();

		/** Set up a network connection listening to port 8000. */
		void SetupConnection();

	public:
		/**
		 * Return an instance of this class, creates one if necessary.
		 * 
		 * @return Streamer* The instance of this class
		 */
		static Streamer* GetStreamer();

		/**
		 * Get the network stream to use for sending data.
		 * This prevents needing to create one each time.
		 * 
		 * @return tcp::iostream* The stored network stream.
		 */
		tcp::iostream* GetStream();
	};
}
