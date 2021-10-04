package com.imageprocess.smartvision.di

import com.imageprocess.computervision.ILowLeverProcess
import com.imageprocess.computervision.LowLevelProcess
import dagger.Binds
import dagger.Module
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@InstallIn(SingletonComponent::class)
@Module
abstract class LowLevelProcessModule {
    @Singleton
    @Binds
    abstract fun bindLowLevelProcess(impl: LowLevelProcess): ILowLeverProcess
}