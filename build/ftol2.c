const int __sse2_available = 0;
/*
__forceinline void __cdecl __sse2_available_init()
{
	__asm mov eax, 1
	__asm cpuid
	__asm shr edx, 26
	__asm and edx, 1
	__asm mov __sse2_available, edx
}
*/
