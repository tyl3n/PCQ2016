// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "OnGameOverData.h"

OnGameOverData::OnGameOverData(int winnerId)
{
	m_winnerId = winnerId;
}

int OnGameOverData::getWinnerId()
{
	return m_winnerId;
}