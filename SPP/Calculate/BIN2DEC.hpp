#pragma once

/// <summary>
/// ������תFieldType����
/// </summary>
/// <typeparam name="FieldType">�ֶ���������</typeparam>
/// <param name="array">ָ��һ���������ڴ�(�豣֤�ڴ����,��תdoubleҪ��ָ֤��������ڴ������а˸��ֽ�)</param>
/// <returns>ת������ֶ�</returns>
template<typename FieldType> requires std::is_arithmetic_v<FieldType>
inline FieldType GetField(unsigned char* array)
{
	return *reinterpret_cast<FieldType*>(array);
}

/// <summary>
/// ��ȡһ������ĳЩ����λ��תΪOutType����
/// </summary>
/// <typeparam name="OutType">ת����������</typeparam>
/// <typeparam name="InType">������������</typeparam>
/// <typeparam name="Cut_Count">��ȡ��λ��</typeparam>
/// <param name="ul">��������</param>
/// <param name="startIndex">��ȡ��ʼλ</param>
/// <returns>�������</returns>
template<typename OutType, typename InType, size_t Cut_Count> requires std::is_integral_v<OutType>&& std::is_integral_v<InType>
inline OutType BitCut(InType ul, int startIndex)
{
	return static_cast<OutType>(((ul >> startIndex) & (~(~0 << Cut_Count))));
}