namespace FieaGameEngine
{
	template <typename T>
	inline void Stack<T>::Push(const T& value)
	{
		mList.PushFront(value);
	}

	template <typename T>
	inline void Stack<T>::Push(T&& value)
	{
		mList.PushFront(std::move(value));
	}

	template <typename T>
	inline void Stack<T>::Pop()
	{
		mList.PopFront();
	}

	template <typename T>
	inline T& Stack<T>::Top()
	{
		return mList.Front();
	}

	template <typename T>
	inline const T& Stack<T>::Top() const
	{
		return mList.Front();
	}

	template <typename T>
	inline std::size_t Stack<T>::Size() const
	{
		return mList.Size();
	}

	template <typename T>
	inline bool Stack<T>::IsEmpty() const
	{
		return mList.IsEmpty();
	}

	template <typename T>
	inline void Stack<T>::Clear()
	{
		return mList.Clear();
	}
}