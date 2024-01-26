#pragma once
//////////////////////////////////////////////////////////////////////
// BytesSpeed.h
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2024.1.1
//////////////////////////////////////////////////////////////////////

#include <deque>
#include <string>

/// <summary>
/// ���ݸ�ʱ����Ѷ�ȡ�����ֽ����������ȡ�ٶȺ�ʣ��ʱ��
/// </summary>

class CBytesSpeed
{
protected:

	typedef struct _INFO
	{
		int64_t timestemp;
		uint64_t current_pos;
	}INFO;

public:
	CBytesSpeed();
	virtual ~CBytesSpeed();

	void reset() {
		queue_list.clear();
	}

	virtual void set_all_bytes(uint64_t file_size) {
		this->bytes_all = file_size;
	}

	auto count()
	{
		return this->bytes_all;
	}

	void add(uint64_t pos);

	virtual uint64_t speed();
	std::string speed_str();
	std::string time_remain();

protected:
	const int MAX_SLOT = 10;
	uint64_t bytes_all = 0;
	std::deque< INFO> queue_list;
};

class CBytesSpeedSector : public CBytesSpeed
{
public:
	CBytesSpeedSector() = default;
	virtual ~CBytesSpeedSector() = default;

	virtual void set_all_bytes(uint64_t file_size) override;
	void set_all_sectors(uint64_t file_size)
	{
		CBytesSpeed::set_all_bytes(file_size);
	}

	virtual uint64_t speed() override;

private:

};

