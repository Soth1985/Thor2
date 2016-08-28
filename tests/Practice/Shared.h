#define CHECK(cond, msg) printf("%s: %s: result:%d\n", __FUNCTION__, #msg, (cond) != 0)

template <class T>
void Swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
T Abs(const T& val)
{
    if (val < 0)
        return -val;
    return val;
}

int BinarySearch(int* array, int arraySize, int val)
{
    int lo = 0;
    int hi = arraySize - 1;
    
    while (hi > lo)
    {
        //int mid = lo + (( hi - lo ) >> 1);
        int mid = ( hi + lo ) >> 1;
        if (array[mid] > val)
        {
            hi = mid - 1;
        }
        else if (array[mid] < val)
        {
            lo = mid + 1;
        }
        else
        {
            return mid;
        }
    }
    
    return -1;
}

int BinaryRange(int* array, int arraySize, int val)
{
    int lo = 0;
    int hi = arraySize - 1;
    
    while (hi > lo)
    {
        //int mid = lo + (( hi - lo ) >> 1);
        int mid = ( hi + lo ) >> 1;
        
        if (val >= array[mid] && val < array[mid + 1])
        {
            return mid;
        }
        else if (val == array[mid + 1])
        {
            return mid + 1;
        }
        
        if (array[mid] > val)
        {
            hi = mid - 1;
        }
        else if (array[mid] < val)
        {
            lo = mid + 1;
        }
        
    }
    
    return -1;
}