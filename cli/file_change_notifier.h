/*! @file file_change_notifier.h
 *
 * */
#pragma once

#include <functional>
#include <string>
#include <Windows.h>

/*! @brief Class that uses a (hopefully)
 *  native API to watch a file for changes and call a user supplied callback
 *  @notice The service can be started and stopped via the applicable functions
 *  (and will also be stopped when destructed if not already done of course)
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
    enum struct change_event : unsigned {
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

    /*! @brief Starts the notification service*/
    void start_watching();

    void stop_watching() noexcept;

    ~file_change_notifier() {
        this->stop_watching();
    }

    /*! @brief Creates a new notifier which will, after being started, call the supplied
     * callback everytime at least one of the spcified events take place
     * @param file_path Full path to the be watched file
     * @param callback Callback which will be called with the files path and
     * the assumed cause of notification
    */
    file_change_notifier(full_path_t file_path,
                         std::function<void(full_path_t, change_event)> callback,
                         change_event events);

private:
    using internal_path_t = std::basic_string<TCHAR>;
    internal_path_t directory_path_;
    internal_path_t file_name_;

    void split_path(full_path_t file_path,
                    internal_path_t& directory_path,
                    internal_path_t& file_name);
};
