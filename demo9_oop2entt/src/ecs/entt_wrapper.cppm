// entt_wrapper.cppm
module;

// ✅ 所有标准库头文件必须在 EnTT 之前显式包含
// 防止 Clang 为不同模块生成不同的匿名内部模块定义
#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <functional>
#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <memory>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <optional>
#include <variant>

// ✅ 然后包含 EnTT
#include <entt/entt.hpp>

export module entt;

// ✅ 导出你项目中实际用到的 EnTT 类型
// 不需要全部导出，按需添加即可
export namespace entt {
    using ::entt::registry;
    using ::entt::entity;
    using ::entt::null;
    using ::entt::view;
    using ::entt::group;
    using ::entt::handle;
    using ::entt::meta_any;
    using ::entt::meta_type;
    // ... 根据项目需要继续添加
}
