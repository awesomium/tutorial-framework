tutorial-framework
==================

Framework to serve as a starting point for Awesomium tutorials.

Please see: http://wiki.awesomium.com/tutorials

### Setting Up on Windows

1. Create a new Visual Studio project (Visual C++ / Win32 project)
2. Click Next and select 'Windows application' and 'Empty project' under settings.
3. Add the files contained within the `src` folder to your project, excluding any filenames ending with `_mac`.
4. Open up project properties and add the includes and proper linker settings for Awesomium.
5. Copy the Awesomium DLLs and binary dependencies to your working path.
6. Follow the tutorial by modifying the `main.cc` file.

### Setting Up on Mac OSX

1. Create a new XCode project (empty Cocoa project).
2. Add the files contained within the `src` folder to your project, excluding any filenames ending with `_win`.
3. Add the dependency for the Awesomium framework.
4. Follow the tutorial by modifying the `main.cc` file.