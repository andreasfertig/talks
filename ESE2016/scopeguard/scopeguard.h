#define CONCATX(x,y) x##y
#define CONCAT(x, y) CONCATX(x,y)

#define ANON_VAR(name) CONCAT(name, __LINE__)

#define RESOLVE_NAME(_0, _1, _2, NAME, ...) NAME

#define SCOPE_GUARD(clsName,...) \
    RESOLVE_NAME(_0, ##__VA_ARGS__, INVALID_SCOPE_GUARD, NAMED_SCOPE_GUARD, ANON_SCOPE_GUARD_WRAPPER)(clsName, __VA_ARGS__)


#define INVALID_SCOPE_GUARD(clsName, ...) \
    static_assert(false, "Error: Invalid use of macro")

#define NAMED_SCOPE_GUARD(clsName, name) \
    clsName name

#define ANON_SCOPE_GUARD_WRAPPER(clsName, _0) \
    ANON_SCOPE_GUARD(clsName)

#define ANON_SCOPE_GUARD(clsName) \
    clsName ANON_VAR(___scopeGuard)
