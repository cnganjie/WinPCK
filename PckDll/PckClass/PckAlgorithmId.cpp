#include <pch.h>
#include "PckAlgorithmId.h"

#if PCK_DEBUG_OUTPUT
#include "PckClassLog.h"
#endif

CPckAlgorithmId::CPckAlgorithmId(uint32_t id) noexcept
{
	SetAlgorithmId(id);
}

void CPckAlgorithmId::SetAlgorithmId(uint32_t id) noexcept
{
	//0	  ����(Jade Dynasty),��������(Perfect World)
	//111 �����ɶ�(Hot Dance Party)
	//121 �ڴ�����(Ether Saga Odyssey)
	//131 ��ħ(Forsaken World)
	//161 ʥ��ʿ(Saint Seiya),Ц������(Swordsman Online)

	switch (id)
	{
	case 111:
		this->PckGuardByte0 = 0xAB12908F;
		this->PckGuardByte1 = 0xB3231902;
		this->PckMaskDword = 0x2A63810E;
		this->PckCheckMask = 0x18734563;
		break;

	default:
		this->PckGuardByte0 = 0xFDFDFEEE + id * 0x72341F2;
		this->PckGuardByte1 = 0xF00DBEEF + id * 0x1237A73;
		this->PckMaskDword = 0xA8937462 + id * 0xAB2321F;
		this->PckCheckMask = 0x59374231 + id * 0x987A223;
		break;
	}
}