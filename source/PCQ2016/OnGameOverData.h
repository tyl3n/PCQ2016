// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EventData.h"

class OnGameOverData : public EventData
{
public:
	OnGameOverData(int winnerId);

	int getWinnerId();

private:
	int m_winnerId;
};
