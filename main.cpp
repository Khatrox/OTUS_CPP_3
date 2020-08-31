#include <iostream>
#include <map>
#include <cmath>
#include <iterator>
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

namespace OwnSingleLinkedList
{
   template<typename T>
   struct node
   {
      T value;
      node<T>* next_node;

      node()
      :
      value{},
      next_node{}
      {

      }
   };

   template<typename T, typename Alloc = std::allocator<T>>
   struct List
   {
      List()
      :
      head{}
      {

      }

     ~List()
     {
        if(head)
        {
           node<T>* next_node{};

           while(head)
           {
              next_node = head->next_node;

              alloc.destroy(head);
              alloc.deallocate(head, 1);

              head = next_node;
           }
        }
     }

   void push_back(T&& t)
   {
      if(!head)
      {
         head = alloc.allocate(1);
         alloc.construct(head);
         head->value = std::forward<T>(t);
      }
      else
      {
         auto node{head};
         while(node->next_node != nullptr)
         {
            node = node->next_node;
         }

         node->next_node = alloc.allocate(1);
         alloc.construct(node->next_node);
         node->next_node->value = std::forward<T>(t);
      }
   }

   T& operator[](std::size_t index)
   {
      if(!head)
      {
         throw std::logic_error("list is empty but tried to proccess operator[]");
      }
      else
      {
         auto ret_val_node{head};
         while(index--)
         {
           ret_val_node = ret_val_node->next_node;
           if(!ret_val_node)
           {
              throw std::logic_error("wrong index in List::operator[]");
           }
         }
          return ret_val_node->value;
      }
   }

   private:
      using NodeAlloc = typename Alloc::template rebind<node<T>>::other;
      node<T>* head;
      NodeAlloc alloc;
   };
}

int factorial(int member_number)
{
   return tgamma(member_number + 1);
}

int main()
{
  std::map<int,int> map_with_standart_alloc;

  for(std::size_t i{}; i < 10; ++i)
  {
      map_with_standart_alloc.insert(std::make_pair(i, factorial(i)));
  }

  std::map<int,int, std::less<int>, own_allocator<std::pair<const int, int>,10>> map_with_own_allocator;

  for(std::size_t i{}; i < 10; ++i)
  {
      map_with_own_allocator.insert(std::make_pair(i,factorial(i)));
  }

  for(std::size_t i{}; i < 10; ++i)
  {
      std::cout << i << ' ' <<  map_with_standart_alloc[i] << '\n';
      std::cout << i << ' ' <<  map_with_own_allocator[i]  << '\n';
  }

  OwnSingleLinkedList::List<int> list_with_standart_alloc;
  for(std::size_t i{}; i < 10; ++i)
  {
      list_with_standart_alloc.push_back(i);
  }

  OwnSingleLinkedList::List<int,own_allocator<int,10>> list_with_own_alloc;
  for(std::size_t i{}; i < 10; ++i)
  {
      list_with_own_alloc.push_back(i);
  }

  for(std::size_t i{}; i < 10; ++i)
  {
      std::cout << list_with_standart_alloc[i] << '\n';
      std::cout << list_with_own_alloc[i] << '\n';
  }

  return 0;
}
