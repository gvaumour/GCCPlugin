# GCC Plugin for static RO data detection
================

This project is a GCC Plugin that performs static read-only data detection. The analysis is limited by the scope of the function.
Some tutorial websites can be found for the GCC plugin API such as http://thinkingeek.com/2015/08/16/a-simple-plugin-for-gcc-part-1/

## Organization of the project 

The apps_test directory contains applications to test the 
The roeval_plugin directory contains the source + lib of the GCC plugin
The plugin_example contains sources of samples of passes for GCC plugin passes implementation

## Compilation 

The plugin depends on the GCC v4.9.2 plugin API 
You have to set the compiler path and the include directory to the plugin API include dir

```bash 
cd <./roeval_plugin>
make
```

## Running:


