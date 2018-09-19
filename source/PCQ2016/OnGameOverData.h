// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EventData.h"

struct OnGameOverData : public EventData
{
	OnGameOverData(int winnerId);
	int getWinnerId();
	int m_winnerId;
};
