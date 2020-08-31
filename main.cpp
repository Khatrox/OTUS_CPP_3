#include "OwnAlloc.h"
#include "List.h"

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
