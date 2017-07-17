#include <tuple>

template<typename Char, Char... CS>
struct basic_string{
    
};

template<typename Name, typename T>
struct parameter{
    T value;
};

template<typename Name>
struct name{
    template<typename T>
    auto operator =(T t){
        return parameter<name, T>{t};
    }
};

template<typename Char, Char... CS>
auto operator "" _arg(){
    return name<basic_string<Char, CS...>>{};
}


template<typename T>
struct default_t{
    T value;
};

template<typename T>
auto default_(T t){
    return default_t<T>{t};
}


template<typename Name, typename... Args>
struct find_name_type;

template<typename Name>
struct find_name_type<Name>{};

template<typename Name, typename T, typename... Args>
struct find_name_type<Name, parameter<Name, T>, Args...>{
    using type = parameter<Name, T>;
};

template<typename Name, typename T, typename... Args>
struct find_name_type<Name, default_t<T>, Args...>{
    using type = default_t<T>;
};


template<typename Name, typename T, typename... Args>
struct find_name_type<Name, T, Args...> : find_name_type<Name, Args...>{};
;



template<
    typename Name,
    typename... Args,
    typename Result = typename find_name_type<Name, Args...>::type
>
auto getArg(Name, Args... args){
    return std::get<Result>(std::make_tuple(args...)).value;
}
