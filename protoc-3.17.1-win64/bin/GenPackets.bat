protoc.exe -I=./ --cpp_out=./ ./Enter.proto Exit.proto Chatting.proto
IF ERRORLEVEL 1 PAUSE