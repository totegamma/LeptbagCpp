#include <tuple>

template <typename Name, typename T>
struct argumentContainer{
	T value;
};

template <typename Char, Char... CS>
struct argumentWrapper{
	template<typename T>
	auto operator =(T t){
		return argumentContainer<argumentWrapper, T>{t};
	}
};

template<typename Char, Char... CS>
auto operator "" _arg(){
	return argumentWrapper<Char, CS...>{};
}


template <typename Char, Char... CS>
struct getArgumentType{
	using type = argumentWrapper<Char, CS...>;
};

template <typename Char, Char... CS>
auto operator"" _unarg(){
	return getArgumentType<Char, CS...>();
}

template<typename T, typename Name, typename... Args, typename result = typename Name::type>
auto getArg(Name, Args... args){
	return std::get<argumentContainer<result, T>>(std::make_tuple(args...)).value;
}

