#ifndef SRC_MATHS
#define SRC_MATHS

namespace maths {
	template <typename T, size_t S>
	class static_vector {
		union {
			T arr[S];
			//TODO: Solve this
			struct {
				T x;
				T y;
				T z;
				T w;
			} val;
		};
	public:
		static_vector<T, S>() {

		}
	};
}

#endif // !SRC_BASE_POINTERS