#pragma once

#include <stddef.h>
#include <EABase/eabase.h>

#if defined(GNARLY_SHARED)
#if defined(EASTL_EXPORT)
#define ARENA_ALLOCATOR_API __declspec(dllexport)
#else
#define ARENA_ALLOCATOR_API __declspec(dllimport)
#endif
#else
#define ARENA_ALLOCATOR_API
#endif

#define EASTLAllocatorDefault eastl_allocator_default
#define EASTLAllocatorType ::engine::eastl_arena_allocator

namespace engine
{

	class IMemoryArena;

	/// Default EASTL allocator for GnarlyEngine.
	//
	/// This allocator automatically looks up the arena it resides in through the
	/// "this" pointer, which means that if you place an eastl::vector in an object
	/// allocated from a memory arena, that vector will allocate its memory from the
	/// same arena.
	/// 
	/// The cost of this magic is an extra cache miss on container construction, so
	/// where performance matters, prefer to pass an arena explicitly to the container
	/// in the constructor.
	///
	/// This has to be all inline'd as there are cyclic dependencies between this file
	/// and eastl headers.

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4100)
#endif

	struct eastl_arena_allocator
	{
		EA_FORCE_INLINE			eastl_arena_allocator(IMemoryArena& a) : m_arena(&a) {}
		EA_FORCE_INLINE explicit	eastl_arena_allocator(const char *name_opt = 0) : m_arena(0) {}
		EA_FORCE_INLINE			eastl_arena_allocator(const eastl_arena_allocator& rhs) : m_arena(rhs.m_arena) {}
		EA_FORCE_INLINE			eastl_arena_allocator(const eastl_arena_allocator& rhs, const char *name_opt) : m_arena(rhs.m_arena) {}

		EA_FORCE_INLINE eastl_arena_allocator&	operator=(const eastl_arena_allocator& x) { verifyArenas(x); m_arena = x.m_arena; return *this; }

		EA_FORCE_INLINE const char* get_name() const { return ""; }
		EA_FORCE_INLINE void        set_name(const char*) {}

		EA_FORCE_INLINE IMemoryArena* get_arena() { ensureArena(*this); return m_arena; }
		EA_FORCE_INLINE const IMemoryArena* get_arena_no_check() const { return m_arena; }

		EA_FORCE_INLINE void        set_arena(IMemoryArena* a) { m_arena = a; }

		EA_FORCE_INLINE static void ensureArena(const eastl_arena_allocator& x) { if (!x.m_arena) const_cast<eastl_arena_allocator&>(x).m_arena = findArena(x); }

		// runtime hook up of Arena allocation functions to avoid Engine.Core dependency on external packages
		ARENA_ALLOCATOR_API void*	allocate(size_t n, int flags = 0);
		ARENA_ALLOCATOR_API void*	allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
		ARENA_ALLOCATOR_API void	deallocate(void* p, size_t n);
		ARENA_ALLOCATOR_API static IMemoryArena* findArena(const eastl_arena_allocator& x);

#if !defined(FB_FINAL)
		EA_FORCE_INLINE		~eastl_arena_allocator() { m_arena = 0; }

		ARENA_ALLOCATOR_API void	verifyArenas(const eastl_arena_allocator& x);
#else
		ARENA_ALLOCATOR_API EA_FORCE_INLINE void	verifyArenas(const eastl_arena_allocator& x) {}
#endif

		typedef void* (EA_PASCAL *eastl_allocator_allocate1_func)(eastl_arena_allocator*, size_t, int);
		typedef void* (EA_PASCAL *eastl_allocator_allocate2_func)(eastl_arena_allocator*, size_t, size_t, size_t, int);
		typedef void (EA_PASCAL *eastl_allocator_deallocate_func)(eastl_arena_allocator*, void*, size_t);
		typedef IMemoryArena* (EA_PASCAL *eastl_allocator_findArena_func)(const eastl_arena_allocator& x);
		typedef void (EA_PASCAL *eastl_allocator_verifyArenas_func)(eastl_arena_allocator*, const eastl_arena_allocator&);

		friend EA_FORCE_INLINE bool operator==(const eastl_arena_allocator &lhs, const eastl_arena_allocator &rhs);

		static ARENA_ALLOCATOR_API void initializeAllocationFunctions(eastl_allocator_allocate1_func f1,
			eastl_allocator_allocate2_func f2,
			eastl_allocator_deallocate_func f3,
			eastl_allocator_findArena_func f4,
			eastl_allocator_verifyArenas_func f5 = nullptr);


	private:
		IMemoryArena* m_arena;

		static ARENA_ALLOCATOR_API eastl_allocator_allocate1_func allocate1Func;
		static ARENA_ALLOCATOR_API eastl_allocator_allocate2_func allocate2Func;
		static ARENA_ALLOCATOR_API eastl_allocator_deallocate_func deallocateFunc;
		static ARENA_ALLOCATOR_API eastl_allocator_findArena_func findArenaFunc;

#if !defined(GNARLY_CONFIG_RELEASE)
		static ARENA_ALLOCATOR_API eastl_allocator_verifyArenas_func verifyArenasFunc;
#endif
		static bool s_allocationMade;
	};

	EA_FORCE_INLINE bool operator==(const eastl_arena_allocator &lhs, const eastl_arena_allocator &rhs)
	{
		eastl_arena_allocator::ensureArena(lhs);
		eastl_arena_allocator::ensureArena(rhs);

		return lhs.m_arena == rhs.m_arena;
	}

	EA_FORCE_INLINE bool operator!=(const eastl_arena_allocator &lhs, const eastl_arena_allocator &rhs)
	{
		return !(lhs == rhs);
	}

}

ARENA_ALLOCATOR_API engine::eastl_arena_allocator* eastl_allocator_default(void);
#ifdef _MSC_VER
#pragma warning(pop)
#endif

