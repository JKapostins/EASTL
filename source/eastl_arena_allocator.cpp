#if !EASTL_NO_MEM_ARENA
#include "EASTL/internal/config.h"
#include "EASTL/eastl_arena_allocator.h"
#if !defined(EA_PLATFORM_MICROSOFT) || defined(EA_PLATFORM_MINGW)
#include <stdlib.h>
#elif defined(EA_PLATFORM_MICROSOFT)
#include <malloc.h>
#endif


namespace eastl_arena_allocator_functions_default
{
	void* allocate(engine::eastl_arena_allocator* /*allocator*/, size_t n, int /*flags*/)
	{
#ifdef EA_PLATFORM_MICROSOFT
		return _aligned_malloc(n, 16);
#else
		void *p = nullptr;
		posix_memalign(&p, 16, n);
		return p;
#endif
	}

	void* allocate(engine::eastl_arena_allocator* /*allocator*/, size_t n, size_t alignment, size_t /*offset*/, int /*flags*/)
	{
#ifdef EA_PLATFORM_MICROSOFT
		return _aligned_malloc(n, alignment);
#else
		void *p = nullptr;
		alignment = alignment < sizeof(void *) ? sizeof(void *) : alignment;
		posix_memalign(&p, alignment, n);
		return p;
#endif
	}

	void deallocate(engine::eastl_arena_allocator* /*allocator*/, void* p, size_t /*n*/)
	{
		if (p)
		{
#ifdef EA_PLATFORM_MICROSOFT
			_aligned_free(p);
#else
			free(p);
#endif
		}
	}

	engine::IMemoryArena* findArena(const engine::eastl_arena_allocator& /*x*/)
	{
		return nullptr;
	}

#if !defined(GNARLY_CONFIG_RELEASE)
	void verifyArenas(engine::eastl_arena_allocator* /*allocator*/, const engine::eastl_arena_allocator& /*x*/)
	{
	}
#endif
}


namespace engine
{
#if !defined(GNARLY_CONFIG_RELEASE)
	eastl_arena_allocator::eastl_allocator_verifyArenas_func	eastl_arena_allocator::verifyArenasFunc = eastl_arena_allocator_functions_default::verifyArenas;
#endif
	eastl_arena_allocator::eastl_allocator_allocate1_func		eastl_arena_allocator::allocate1Func = eastl_arena_allocator_functions_default::allocate;
	eastl_arena_allocator::eastl_allocator_allocate2_func		eastl_arena_allocator::allocate2Func = eastl_arena_allocator_functions_default::allocate;
	eastl_arena_allocator::eastl_allocator_deallocate_func		eastl_arena_allocator::deallocateFunc = eastl_arena_allocator_functions_default::deallocate;
	eastl_arena_allocator::eastl_allocator_findArena_func		eastl_arena_allocator::findArenaFunc = eastl_arena_allocator_functions_default::findArena;
	bool eastl_arena_allocator::s_allocationMade = false;

	void* eastl_arena_allocator::allocate(size_t n, int flags)
	{
		s_allocationMade = true;
		return allocate1Func(this, n, flags);
	}

	void* eastl_arena_allocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
	{
		s_allocationMade = true;
		return allocate2Func(this, n, alignment, offset, flags);
	}

	void eastl_arena_allocator::deallocate(void* p, size_t n)
	{
		deallocateFunc(this, p, n);
	}

	IMemoryArena* eastl_arena_allocator::findArena(const eastl_arena_allocator& x)
	{
		return findArenaFunc(x);
	}

#if !defined(GNARLY_CONFIG_RELEASE)
	void eastl_arena_allocator::verifyArenas(const eastl_arena_allocator& x)
	{
		verifyArenasFunc(this, x);
	}
#endif

	void eastl_arena_allocator::initializeAllocationFunctions(eastl_allocator_allocate1_func f1, eastl_allocator_allocate2_func f2, eastl_allocator_deallocate_func f3, eastl_allocator_findArena_func f4, eastl_allocator_verifyArenas_func f5)
	{
		EASTL_ASSERT_MSG(!s_allocationMade, "Cannot change allocation functions after any allocations have been made.");
		allocate1Func = f1;
		allocate2Func = f2;
		deallocateFunc = f3;
		findArenaFunc = f4;
#if !defined(GNARLY_CONFIG_RELEASE)
		verifyArenasFunc = f5;
#endif
		EA_UNUSED(f5);
	}
}


EASTL_API engine::eastl_arena_allocator* eastl_allocator_default(void)
{
	static engine::eastl_arena_allocator defaultArenaAllocator;
	return &defaultArenaAllocator;
}

#endif // EASTL_NO_MEM_ARENA