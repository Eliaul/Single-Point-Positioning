#pragma once

#include <format>
#include "../CoordinateSystem/XYZCoordinate.hpp"

//-------正在施工-------
//自定义XYZ的格式化输出,程序中未使用
template<>
struct std::formatter<XYZCoordinate> : std::formatter<double>
{
	bool isMyFormat{ true };
	bool isBrackets{ false };
	
	constexpr auto parse(std::format_parse_context& ctx)
	{
		auto pos = ctx.begin(), end = ctx.end();
		if (pos == end)
		{
			return end;
		}
		if ((pos != end && *pos == '}'))
		{
			return --end;
		}
		if (pos != end && *pos == '%')
		{
			++pos;
			if (pos != end && (*pos == 'b' || *pos == 'B'))
			{
				isBrackets = true;
				return ++pos;
			}
			throw std::format_error("Invalid format");
		}
		isMyFormat = false;
		return std::formatter<double>::parse(ctx);
	}

	auto format(const XYZCoordinate& xyz, std::format_context& ctx)
	{
		if (isMyFormat)
		{
			if (isBrackets)
			{
				auto str = std::format("({:12.4f},{:12.4f},{:12.4f})m", xyz(1), xyz(2), xyz(3));
				return std::format_to(ctx.out(), "{}", str);
			}
			auto str = std::format("{:12.4f} {:12.4f} {:12.4f}", xyz(1), xyz(2), xyz(3));
			return std::format_to(ctx.out(), "{}", str);
		}
		std::formatter<double>::format(xyz(1), ctx);
		std::format_to(ctx.out(), " ");
		std::formatter<double>::format(xyz(2), ctx);
		std::format_to(ctx.out(), " ");
		std::formatter<double>::format(xyz(3), ctx);
		return ctx.out();
	}
};