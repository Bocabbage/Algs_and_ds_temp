//优先队列(priority queue）模板类
template<typename T>
void exchange(T* a, T* b)
{
	T temp = *a;
	*a = *b;
	*b = temp;
}

template<typename T>
class MaxPQ
{
private:
	T* pq;
	int N;
	int msize;
	void swim(int k);	//由下至上的堆有序化
	void sink(int k);	//由上至下的堆有序化
public:
	MaxPQ(){msize=50;pq=new T[msize+1];N=0;}
	MaxPQ(int n){msize=n;pq=new T[msize+1];N=0;}	
	~MaxPQ(){delete[] pq;}
	T max(){if(N!=0)return pq[1];}
	bool isEmpty(){return N==0;}
	int size(){return N;}
	bool insert(T v);
	T delMax();
	void show();
};

template<typename T>
void MaxPQ<T>::swim(int k)
{
	while(k>1 && pq[k]>pq[k/2])
	{
		exchange<T>(pq+k,pq+k/2);
		k/=2;
	}
}

template<typename T>
void MaxPQ<T>::sink(int k)
{
	while(2*k<=N)
	{	
		int j=2*k;
		if(j<N && pq[j]<pq[j+1])j++;
		if(pq[k]>=pq[j])break;
		exchange<T>(pq+k,pq+j);
		k=j;
	}
}

template<typename T>
bool MaxPQ<T>::insert(T v)
{
	if(N==msize)return false;
	pq[++N]=v;
	swim(N);
}

template<typename T>
T MaxPQ<T>::delMax()
{
	if(isEmpty<T>())exit();
	T max=max<T>();
	exchange<T>(pq+1,pq+N);
	pq[N--]=0;
	sink(1);
	return max;
}

template<typename T>
void MaxPQ<T>::show()
{
	for (int i = 1; i <= N; i++)
		cout << pq[i] << ' ';
}

