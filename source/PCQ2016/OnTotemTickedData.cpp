// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "OnTotemTickedData.h"

OnTotemTickedData::OnTotemTickedData(int32 figtherId)
{
	m_fighterId = figtherId;
}

int OnTotemTickedData::getFigtherId()
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), m_fighterId);
	return m_fighterId;
}
