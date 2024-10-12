#pragma once

#ifndef EPOLLUTILS_HPP
# define EPOLLUTILS_HPP

#include <netinet/tcp.h>

namespace EpollUtils
{
	/// @brief Adds a file descriptor to the specified epoll instance to monitor it for the given events.
	/// @param epollFd The file descriptor of the epoll instance.
	/// @param fd The file descriptor to add to the epoll instance.
	/// @param events The events to monitor on the file descriptor (e.g., EPOLLIN, EPOLLOUT).
	void EpollAdd(int epollFd, int fd, uint32_t events);

	/// @brief Removes a file descriptor from the specified epoll instance, stopping its monitoring.
	/// @param epoll_fd The file descriptor of the epoll instance.
	/// @param fd The file descriptor to remove from the epoll instance.
	void EpollDelete(int epoll_fd, int fd);

	/// @brief Check if passed events have error
	/// @param events The event flags returned from epoll_wait.
	/// @return True if an error event occurred, false otherwise.
	bool EpollCheckEventError(uint32_t events);

	/// @brief Creates a new epoll instance and returns its file descriptor.
	/// @return The file descriptor of the created epoll instance, or -1 on error.
	int EpollInit();

} // namespace EpollUtils


#endif // !EPOLLUTILS_HPP