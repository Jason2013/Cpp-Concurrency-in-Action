#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

template<typename Iterator,typename T>
struct accumulate_block
{
    void operator()(Iterator first,Iterator last,T& result)
    {
        result=std::accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
    auto const length=std::distance(first,last);

    if(!length)
        return init;

    decltype(length) min_per_thread=25;
    decltype(length) max_threads=
        (length+min_per_thread-1)/min_per_thread;

    decltype(length) hardware_threads=
        std::thread::hardware_concurrency();

    decltype(length) num_threads=
        std::min(hardware_threads!=0?hardware_threads:2,max_threads);

    decltype(length) block_size=length/num_threads;

    std::vector<T> results(num_threads);
    std::vector<std::thread>  threads(num_threads-1);

    Iterator block_start=first;
    for (decltype(results.size()) i=0; i<(results.size()-1); ++i)
    {
        Iterator block_end=block_start;
        std::advance(block_end,block_size);
        threads[i]=std::thread(
            accumulate_block<Iterator,T>(),
            block_start,block_end,std::ref(results[i]));
        block_start=block_end;
    }
    accumulate_block<Iterator,T>()(block_start,last,results[results.size()-1]);
    
    std::for_each(threads.begin(),threads.end(),
        std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(),results.end(),init);
}

int main()
{
    std::vector<int> vi;
    for(int i=0;i<10;++i)
    {
        vi.push_back(10);
    }
    int sum=parallel_accumulate(vi.begin(),vi.end(),5);
    std::cout<<"sum="<<sum<<std::endl;
}
