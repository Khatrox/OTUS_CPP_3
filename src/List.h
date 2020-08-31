#include <iostream>
#include <map>
#include <cmath>
#include <iterator>

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
