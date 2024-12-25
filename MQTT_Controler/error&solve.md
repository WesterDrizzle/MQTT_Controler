## Error:	

 ### cmake找不到工作路径 

## solve:

### set(CMAKE_PREFIX_PATH "C:/Qt/6.7.3/mingw_64")

## Error:	

 ### cmake不能链接Mqtt库

## solve:

### find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Mqtt)

