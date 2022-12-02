#pragma once

/// <summary>
/// 二进制转FieldType类型
/// </summary>
/// <typeparam name="FieldType">字段数据类型</typeparam>
/// <param name="array">指向一块连续的内存(需保证内存对齐,即转double要保证指向的连续内存至少有八个字节)</param>
/// <returns>转换后的字段</returns>
template<typename FieldType> requires std::is_arithmetic_v<FieldType>
inline FieldType GetField(unsigned char* array)
{
	return *reinterpret_cast<FieldType*>(array);
}

/// <summary>
/// 截取一个数的某些比特位并转为OutType类型
/// </summary>
/// <typeparam name="OutType">转出数据类型</typeparam>
/// <typeparam name="InType">输入数据类型</typeparam>
/// <typeparam name="Cut_Count">截取的位数</typeparam>
/// <param name="ul">输入数据</param>
/// <param name="startIndex">截取开始位</param>
/// <returns>输出数据</returns>
template<typename OutType, typename InType, size_t Cut_Count> requires std::is_integral_v<OutType>&& std::is_integral_v<InType>
inline OutType BitCut(InType ul, int startIndex)
{
	return static_cast<OutType>(((ul >> startIndex) & (~(~0 << Cut_Count))));
}