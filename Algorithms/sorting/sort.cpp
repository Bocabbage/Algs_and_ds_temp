#inlcude<ctime>		//for randomizing of partition
template<typename T>
void exchange(T* a, T* b)
{
	T temp = *a;
	*a = *b;
	*b = temp;
}


template<typename T>
void SelectSort(T a[], int n, bool reverse = true)
{
	//reverse=true:smallest-->biggest
	if (reverse)
	{		
		for (int i = 0; i < n; ++i)
		{	
			T min_index = i;
			for (int j = i; j < n; ++j)
				if (a[min_index]>a[j])min_index = j;
			exchange<T>(a + i, a + min_index);
		}
	}
	else
	{		
		for (int i = 0; i < n; ++i)
		{
			T max_index = i;
			for (int j = i; i < n; ++j)
				if (a[max_index]<a[j])max_index = j;
			exchange<T>(a + i, a + max_index);
		}
	}
}

template<typename T>
void InsertSort(T a[],int n,bool reverse = true)
{
	//reverse=true:smallest-->biggest
	if(reverse)
		for (int i = 1; i < n; ++i)
			for(int j = i; j > 0 && a[j]<a[j-1]; --j)	
				exchange<T>(a+j,a+j-1);
	/*
		or:(more efficient)

		for (int i = 1; i < n; ++i)
		{
			T temp = a[i];
			int j;						#needs the j index after the second-cur
			for (j = i; j > 0; --j)
			{

				if (temp < a[j - 1])a[j] = a[j - 1];
				else break;
			}
			a[j] = temp;
		}
	*/
	else
		for (int i = 1; i < n; ++i)
			for(int j = i; j > 0 && a[j]>a[j-1]; --j)
				exchange<T>(a+j,a+j-1);

}

template<typename T>
void ShellSort(T a[],int n,bool reverse = true)
{
	if(reverse)
	{
		int h=1;				
		while(h<n/3)h=3*h+1;	//use the arr 1/2(3^k-1),from n/3 to 1
		while(h>=1)
		{
			for(int i = h; i < n; i++)
				for(int j = i; j >= h && a[j]<a[j-h]; j-=h)
					exchange<T>(a+j,a+j-h);
			h=h/3;
		}
	}

	else
	{
		int h=1;
		while(h<n/3)h=3*h+1;
		while(h>=1)
		{
			for(int i = h; i < n; i++)
				for(int j = i; j >=h && a[j]>a[j-h]; j-=h)
					exchange<T>(a+j,a+j-h);
		}
	}
}

/***********************/
template<typename T>
void merge(T a[],int lo,int mid,int hi,bool reverse)	//in-place merge
{
	//merge the a[lo...mid] and a[mid+1...hi]
	T aux[hi+1];
	int i=lo;
	int j=mid+1;

	for (int k = 0; k < hi+1; ++k)	//copy a[] to aux[]
		aux[k]=a[k];
	if(reverse)			
		for (int k = 0; k < hi+1; ++k)
		{
			if(i>mid)a[k]=aux[j++];		//remains nothing in a[lo...mid]
			else if(j>hi)a[k]=aux[i++]; //remains nothing in a[mid+1...hi]
			else if(aux[j]<aux[i])a[k]=aux[j++];//compare and select
			else a[k]=aux[i++];					//compare and select
		}
	else
		for (int k = 0; k < hi+1; ++k)
		{
			if(i>mid)a[k]=aux[j++];		//remains nothing in a[lo...mid]
			else if(j>hi)a[k]=aux[i++]; //remains nothing in a[mid+1...hi]
			else if(aux[j]>aux[i])a[k]=aux[j++];//compare and select
			else a[k]=aux[i++];					//compare and select
		}

}

template<typename T>
void MergeSort(T a[],int n,bool reverse=true)
{
	//from top to buttom method
	if(n>1)
	{
		MergeSort<T>(a,n/2,reverse);
		MergeSort<T>(a+n/2,n-n/2,reverse);
		merge<T>(a,0,n/2,n,reverse);
	}

}
//Another method
/*
void MergeSort_idx(T a[],int lo,int hi,reverse=true)
{
	if(hi<=lo)return;
	int mid=lo+(hi-lo)/2;
	MergeSort_idx<T>(a,lo,mid,reverse);
	MergeSort_idx<T>(a,mid+1,hi,reverse);
	merge<T>(a,lo,mid,hi,reverse);
}
*/

template<typename T>
void MergeSortBU(T a[],int n,bool reverse=true)	//better for list to achive
{
	for(int sz=1;sz<n;sz=sz+sz)
		for(int lo=0;lo<n-sz;lo+=sz+sz)
			merge<T>(a,lo,lo+sz-1,(lo+sz+sz-1<n-1)?lo+sz+sz:n-1,reverse);
}
/***********************/

/***********************/
template<typename T>
int partition(T a[], int lo, int hi, bool reverse = true)
{

	srand((unsigned)time(NULL));
	int r = rand() % (hi - lo + 1) + lo;
	exchange<T>(a + lo, a + r);

	T alo = a[lo];
	int i = lo;
	int j = hi+1;

	if (reverse)
		while (true)
		{
			while (a[++i] < alo)if (i == hi)break;
			while (a[--j] > alo)if (j == lo)break;
			if (i >= j)break;
			exchange<T>(a + i, a + j);
		}
	else
		while (true)
		{
			while (a[++i] > alo)if (i == hi)break;
			while (a[--j] < alo)if (j == lo)break;
			if (i >= j)break;
			exchange<T>(a + i, a + j);
		}

	exchange<T>(a + lo, a + j);
	return j;
}

template<typename T>
void QuickSort(T a[], int lo, int hi, bool reverse = true)
{
	if (hi <= lo)return;	
	//optimalization: 
	//if(hi<=lo+M){InsertSort<T>(a+lo,hi-lo+1,reverse)};
	int j = partition<T>(a, lo, hi, reverse);
	QuickSort<T>(a, lo, j - 1, reverse);
	QuickSort<T>(a, j + 1, hi, reverse);
}

template<typename T>
void Quick3way(T a[],int lo,int hi)
{
	/*三取样切分：适用于含有大量重复元素的序列*/
	if(hi <= lo)return;
	int lt=lo;
	int i=lo+1;
	int gt=hi;
	T alo=a[lo];
	while(i<=gt)
	{
		if(a[i]<alo){exchange<T>(a+i,a+lt);++lt;++i;}
		else if(a[i]>alo){exchange<T>(a+i,a+gt);--gt;}//此处不可i++：需再检测此时a[i]是否>alo
		else ++i;
	}
	Quick3way<T>(a,lo,lt-1);
	Quick3way<T>(a,gt+1,hi);
}
/***********************/
template<typename T>
void sink(T pq[], int N, int k)
{
	int K = k + 1;		//将索引由[0...N-1]变为[1...N]
	int n = N - 1;
	while (2 * K <= n)
	{
		int j = 2 * K;
		if (j<n && pq[j-1]<pq[j])j++;
		if (pq[K-1] >= pq[j-1])break;
		exchange<T>(pq+K-1, pq+j-1);
		K = j;
	}
}

template<typename T>	//升序:将a[0]到a[N-1]排序
void HeapSort(T a[], int N)
{
	for (int k = (N-1) / 2; k >= 0; k--)
		sink<T>(a, N, k);
	while (N > 1)
	{
		exchange<T>(a, a + (N-1));
		--N;
		sink<T>(a, N, 0);
	}
}

