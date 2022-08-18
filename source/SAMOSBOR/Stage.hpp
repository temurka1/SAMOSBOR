#pragma once

#include <functional>
#include <pipeline/fn.hpp>

#include "ResultOr.hpp"

namespace SAMOSBOR::workflow
{
    namespace core = SAMOSBOR::core;

    template <typename Ret, typename Arg>
    using stage_fn_internal = std::function<core::ResultOr<Ret>(core::ResultOr<Arg>)>;

    template <typename Ret, typename Arg>
    using stage_fn = std::function<core::ResultOr<Ret>(Arg)>;

    template <typename Ret, typename Arg> 
    class Stage : public pipeline::fn<stage_fn_internal<Ret, Arg>>
    {
    public:
        Stage(stage_fn<Ret, Arg> stageFn) : 
            pipeline::fn<stage_fn_internal<Ret, Arg>>([&stageFn](const core::ResultOr<Arg>& arg) -> core::ResultOr<Ret>
            {
                return arg.Ok() ? stageFn(arg.Value()) : core::ResultOr<Ret>(arg.Res());
            })
        {
        }
    };
}