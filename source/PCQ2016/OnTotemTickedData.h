// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EventData.h"

class OnTotemTickedData : public EventData
{
public:
	OnTotemTickedData(int32 figtherId);

	int32 getFigtherId();

private:
	int32 m_fighterId;
};
