#include <exception>
#include <stdexcept>
#include <stdlib.h>

template<typename T, std::size_t N>
struct own_allocator
{
using value_type = T;

template<typename U>
struct rebind
{
    using other = own_allocator<U,N>;
};

   own_allocator()
   :
   memory_pool{},
   gave_objects{}
   {
       auto reserve_block = std::malloc(N * sizeof(T));
       
       if(!reserve_block)
       {
          throw std::bad_alloc();
       }
       else
       {
          memory_pool = reserve_block;
       }

   }

   ~own_allocator()
   {
       free(memory_pool);
   }

   T* allocate(std::size_t n)
   {
       if(gave_objects == N)
       {
           free(memory_pool);
           throw std::logic_error("gave_objects == N");
       }
       else
       {
           auto ret_val = reinterpret_cast<T*>(memory_pool) + gave_objects;
           ++gave_objects;
           return ret_val;
       }
   }

   void deallocate(T* p, std::size_t n)
   {
   }

   template<typename U, typename ...Args>
   void construct(U* p, Args&&... args)
   {
      new(p) U(std::forward<Args>(args)...);
   }

   template<typename U>
   void destroy(U*p)
   {
      p->~U();
   }

private:

   void* memory_pool;
   std::size_t gave_objects;

};
