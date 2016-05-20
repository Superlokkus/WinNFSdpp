import qbs

CppApplication {
    consoleApplication: true

    name: "FileChangeTest"

    files: [
        "file_change_test.cpp",
        "../../cli/file_change_notifier.h",
        "../../cli/file_change_notifier.cpp"
    ]

    Depends { name: "WinNFSdppLib" }
    Depends { name: "GoogleTestMain" }

}
