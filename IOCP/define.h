#define size16(val)		static_cast<short>(sizeof(val))
#define size32(val)		static_cast<int>(sizeof(val))
#define len16(arr)		static_cast<short>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)		static_cast<int>(sizeof(arr)/sizeof(arr[0]))

