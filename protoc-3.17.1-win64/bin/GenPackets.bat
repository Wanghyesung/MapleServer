protoc.exe -I=./ --cpp_out=./ ./Enter.proto Exit.proto Collision.proto Equip.proto Input.proto Map.proto ObjectState.proto Transform.proto GameObject.proto
IF ERRORLEVEL 1 PAUSE