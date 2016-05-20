/*! @file file_change_notifier.h
 *
 * */
#pragma once

#include <functional>
#include <string>
#include <atomic>
#include <future>
#include <Windows.h>

/*! @brief Class that uses a (hopefully)
 *  native API to watch a file for changes and call a user supplied callback
 *  The callback service can be started/stopped via the applicable functions
 *  Also the callback will only be called in 1 thread by this class
 *
 * @TODO Change the callback function to static an just give the atomic and a copy
 * of the callback function, to make race conditions impossible
 */
class file_change_notifier
{
public:
    using full_path_t = std::wstring;
    using file_name_t = std::wstring;
    using directory_path_t = std::wstring;

    /*! @brief Events can be combined via the flag operations
     * @attention Be aware that due the nature of the APIs, this is intended
     * for specifying the events you AT LEAST want a notification. Be ready to
     * get notifications even when nothing changed or more than specified. */
    enum change_event : unsigned {
        file_name = 0x1u, ///<  Includes renaming, creating, or deleting a file
        file_attributes = 0x2u, //< Includes timestamp, security attributes and so on
        file_size = 0x4u,
        all = 0x0u,
    };

    file_change_notifier() = delete;

    /*! @brief Creates a new notifier which will, after being started, call the supplied
     * callback everytime, the file changes in any sort
     * @param file_path Full path to the to be watched file
     * @param callback Callback which will be called with the files path
     * at least any time the file has changed
    */
    file_change_notifier(full_path_t file_path,
                         std::function<void(full_path_t)> callback)
        : file_change_notifier(file_path,std::bind(callback,std::placeholders::_1),
                               change_event::all)
    {}

    /*! @brief Starts/Resume to call the callback for any change since constructing*/
    void start_watching();

    /*! @brief Stops the issuing of callbacks for now*/
    void stop_watching() noexcept;

    ~file_change_notifier() {
        this->stop_watching();
    }

    /*! @brief Creates a new notifier which will, after being started, call the supplied
     * callback everytime at least one of the spcified events take place
     * @param file_path Full path to the be watched file
     * @param callback Callback which will be called with a (!) usable path to the file
     * @warning The path given to the callback might differ from the one supplied to the
     * constructor of this class, but refers still to the same file.
    */
    file_change_notifier(full_path_t file_path,
                         std::function<void(full_path_t, change_event)> callback,
                         change_event events);

private:
    using internal_path_t = std::wstring;
    internal_path_t directory_path_;
    internal_path_t file_name_;
    std::function<void(full_path_t, change_event)> callback_;
    change_event events_;
    HANDLE notification_handle_;
    std::atomic<bool> continue_waiting_;
    std::future<void> waiting_future;

    void split_path(full_path_t file_path,
                    internal_path_t& directory_path,
                    internal_path_t& file_name);
    void waiting_function();
};



