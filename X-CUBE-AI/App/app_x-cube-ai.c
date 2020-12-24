
#ifdef __cplusplus
 extern "C" {
#endif
/**
  ******************************************************************************
  * @file           : app_x-cube-ai.c
  * @brief          : AI program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "app_x-cube-ai.h"
#include "main.h"
#include "ai_datatypes_defines.h"
#include "stdio.h"

/* USER CODE BEGIN includes */
/* USER CODE END includes */
/* USER CODE BEGIN initandrun */
#include <stdlib.h>
#define input_SIZE 270 // (90,3,1)
#define output_SIZE 6

/* Global handle to reference the instance of the NN */
static ai_handle hargithub = AI_HANDLE_NULL;
static ai_buffer ai_input[AI_HARGITHUB_IN_NUM] = AI_HARGITHUB_IN ;
static ai_buffer ai_output[AI_HARGITHUB_OUT_NUM] = AI_HARGITHUB_OUT ;

 AI_ALIGNED(4)
 static ai_u8 activations[AI_HARGITHUB_DATA_ACTIVATIONS_SIZE];
 
/*
 * Init function to create and initialize a NN.
 */
int aiInit(const ai_u8* activations)
{
    ai_error err;

    /* 1 - Specific AI data structure to provide the references of the
     * activation/working memory chunk and the weights/bias parameters */
    const ai_network_params params = {
            AI_HARGITHUB_DATA_WEIGHTS(ai_hargithub_data_weights_get()),
            AI_HARGITHUB_DATA_ACTIVATIONS(activations)
    };

    /* 2 - Create an instance of the NN */
    err = ai_hargithub_create(&hargithub, AI_HARGITHUB_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) {
	    return -1;
    }

    /* 3 - Initialize the NN - Ready to be used */
    if (!ai_hargithub_init(hargithub, &params)) {
        err = ai_hargithub_get_error(hargithub);
        ai_hargithub_destroy(hargithub);
        hargithub = AI_HANDLE_NULL;
	    return -2;
    }

    return 0;
}

/*
 * Run function to execute an inference.
 */
int aiRun(const void *in_data, void *out_data)
{
    ai_i32 nbatch;
    ai_error err;

    /* Parameters checking */
    if (!in_data || !out_data || !hargithub)
        return -1;

    /* Initialize input/output buffer handlers */
    ai_input[0].n_batches = 1;
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].n_batches = 1;
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    /* 2 - Perform the inference */
    nbatch = ai_hargithub_run(hargithub, &ai_input[0], &ai_output[0]);
    if (nbatch != 1) {
        err = ai_hargithub_get_error(hargithub);
        // ...
        return err.code;
    }

    return 0;
}
/* USER CODE END initandrun */

/*************************************************************************
  *
  */
void MX_X_CUBE_AI_Init(void)
{
    /* USER CODE BEGIN 0 */
	aiInit(activations);
    /* USER CODE END 0 */
}

void MX_X_CUBE_AI_Process(void)
{
    /* USER CODE BEGIN 1 */
	
	ai_float out_data[output_SIZE] ;
	
	/*
	Downstairs:
	*/
//	ai_float in_data[90][3][1]={
//{{-1.0760075},{6.2789803},{3.9499009}},
//{{-1.9204689999999998},{15.023244},{6.851035}},
//{{-0.6537767},{8.771504},{4.5628166}},
//{{-0.50395286},{4.6036777},{1.56634}},
//{{-0.6946376999999999},{6.5105260000000005},{3.1463002999999996}},
//{{-0.7627395},{7.9679036},{2.070293}},
//{{-3.0237172},{11.264028},{4.099725}},
//{{-0.8036005},{15.213928},{0.88532263}},
//{{-0.50395286},{10.460427000000001},{-0.53119355}},
//{{0.27240697},{8.471855999999999},{1.4982381999999999}},
//{{2.5333846},{6.9327570000000005},{1.7297841}},
//{{-2.6832085},{8.349273},{1.2666923}},
//{{-0.53119355},{16.821129},{3.9090397000000006}},
//{{1.56634},{8.117727},{2.070293}},
//{{-2.3699405},{2.5333846},{1.0760075}},
//{{-5.7477865},{12.762266},{4.290409599999999}},
//{{3.4050870000000004},{7.3549875999999985},{-2.0294318}},
//{{-3.173541},{8.539958},{0.5720546}},
//{{-1.3075534},{9.152874},{0.19068487}},
//{{-1.2258313},{9.656827},{0.95342433}},
//{{-0.84446156},{10.800936},{0.50395286}},
//{{-1.7297841},{12.571581},{-0.29964766}},
//{{-1.0760075},{12.762266},{2.2201166000000003}},
//{{-0.46309182},{6.0201936},{1.3075534}},
//{{-2.4516625},{7.8861814},{2.6423476000000004}},
//{{-1.8387468999999996},{8.076866},{1.6889231}},
//{{-1.3075534},{6.782933},{1.9885708}},
//{{-1.0351465},{7.8589406},{1.9885708}},
//{{-3.1463002999999996},{9.724928},{3.5957718}},
//{{-0.14982383},{9.697687},{-0.46309182}},
//{{-0.42223078},{16.208214},{0.7218784}},
//{{-0.61291564},{9.343558},{-0.6537767}},
//{{0.50395286},{8.689782000000001},{2.152015}},
//{{2.152015},{5.706926},{1.9477096999999999}},
//{{-3.0237172},{10.841797},{5.5571017}},
//{{-1.3075534},{14.09706},{6.9327570000000005}},
//{{-0.0},{5.1348709999999995},{2.6423476000000004}},
//{{-0.46309182},{4.140586},{2.7921712000000003}},
//{{-1.56634},{7.205164},{1.5390993}},
//{{-2.4925237},{10.56939},{3.8273177000000005}},
//{{0.313268},{13.565866},{3.2961242}},
//{{-0.14982383},{6.469665},{-1.607201}},
//{{-0.0},{7.6273947},{2.3426998}},
//{{0.27240697},{9.956474},{2.2609777}},
//{{-0.53119355},{6.701211},{1.9885708}},
//{{-0.84446156},{8.662541000000001},{3.8273177000000005}},
//{{-0.23154591},{12.762266},{6.782933}},
//{{0.10896278},{6.823794400000001},{2.8738932999999998}},
//{{-1.9477096999999999},{4.903325},{1.1168685}},
//{{-0.6537767},{6.823794400000001},{3.0645783}},
//{{-1.1441092},{11.454713},{2.4516625}},
//{{-0.29964766},{15.785982999999998},{1.2666923}},
//{{-1.4165162},{8.499097},{-0.42223078}},
//{{-1.0351465},{7.9679036},{1.0760075}},
//{{1.4573771999999998},{3.7183550000000003},{2.070293}},
//{{-2.2609777},{6.851035},{3.4050870000000004}},
//{{-1.2666923},{12.830366999999999},{6.66035}},
//{{-1.3075534},{6.5105260000000005},{3.336985}},
//{{-0.5720546},{2.9147543999999996},{0.7627395}},
//{{-0.88532263},{9.80665},{3.5685309999999997}},
//{{-3.1463002999999996},{12.14935},{3.8273177000000005}},
//{{-0.14982383},{15.894946},{3.0645783}},
//{{-1.6889231},{10.460427000000001},{1.1849703}},
//{{0.6537767},{4.6036777},{0.92618364}},
//{{0.27240697},{8.049625},{3.4050870000000004}},
//{{-2.3426998},{15.363751999999998},{3.336985}},
//{{-3.2961242},{9.112013000000001},{4.671779}},
//{{-0.42223078},{3.5957718},{0.6537767}},
//{{-1.7297841},{9.043911},{4.2086873}},
//{{-2.7513103},{12.680544},{3.8273177000000005}},
//{{0.7627395},{16.126492000000002},{1.56634}},
//{{-1.7978859},{9.507003},{0.61291564}},
//{{1.8796080000000002},{7.014479},{2.2201166000000003}},
//{{0.19068487},{6.701211},{2.6423476000000004}},
//{{-4.140586},{15.132206},{5.1348709999999995}},
//{{-3.0645783},{11.917805},{5.47538}},
//{{-1.1849703},{3.5276701},{0.61291564}},
//{{0.7627395},{6.66035},{2.982856}},
//{{-3.4459480000000005},{7.3958488},{2.9147543999999996}},
//{{1.4982381999999999},{15.431854000000001},{-1.3075534}},
//{{-0.38136974},{11.645397},{1.6889231}},
//{{0.9942854},{7.8589406},{1.3756552}},
//{{1.2666923},{7.5456724},{1.9477096999999999}},
//{{-2.7513103},{5.243834},{0.3405087}},
//{{0.040861044},{17.502148000000002},{1.7297841}},
//{{0.7627395},{9.956474},{1.2666923}},
//{{-0.88532263},{5.8976107},{-1.334794}},
//{{-0.46309182},{11.34575},{5.09401}},
//{{0.5720546},{4.140586},{-1.6480621}},
//{{-1.9885708},{9.112013000000001},{0.08172209}}};
	/*
	Jogging:
	*/
//	ai_float in_data [90][3][1] = {
//{{11.604536},{17.079916},{1.334794}},
//{{-3.173541},{14.015338},{5.706926}},
//{{0.61291564},{1.1168685},{2.5606253}},
//{{-7.8861814},{19.57244},{1.9885708}},
//{{3.1463002999999996},{5.243834},{4.671779}},
//{{-3.0237172},{-4.3312707},{-3.336985}},
//{{-0.08172209},{11.917805},{-7.8861814}},
//{{-1.0351465},{14.818938000000001},{4.6036777}},
//{{-2.4516625},{2.5333846},{3.4868089999999996}},
//{{-1.3756552},{2.070293},{-0.19068487}},
//{{-2.4925237},{19.57244},{6.469665}},
//{{1.4573771999999998},{-5.243834},{-4.372132}},
//{{-1.4165162},{9.80665},{5.7477865}},
//{{-1.2666923},{14.709975},{6.2108784}},
//{{-3.6774937999999993},{3.173541},{3.7864565999999997}},
//{{1.8387468999999996},{2.7649305},{-1.7570249}},
//{{-1.2666923},{19.313654},{6.3198414000000005}},
//{{2.4108016},{-7.654635400000001},{-6.1291566}},
//{{-0.61291564},{16.358038},{4.944186}},
//{{0.040861044},{17.502148000000002},{2.5333846}},
//{{-7.654635400000001},{7.8180795},{4.372132}},
//{{-1.2666923},{0.7218784},{0.8036005}},
//{{-5.012288},{19.57244},{5.5162406}},
//{{1.9477096999999999},{2.7921712000000003},{2.070293}},
//{{-5.0531489999999994},{1.6480621},{7.6273947}},
//{{9.384419},{13.443283},{1.0351465}},
//{{-5.434519},{13.211738},{6.4424243}},
//{{-0.61291564},{1.8796080000000002},{1.4165162}},
//{{4.7126402999999994},{-6.5513873},{-6.0201936}},
//{{-1.7570249},{9.302697},{-6.428804}},
//{{-0.9125633},{10.501288},{-0.27240697}},
//{{2.6014863999999998},{19.381754},{4.440233}},
//{{5.7886477},{3.214402},{1.1441092}},
//{{-1.9885708},{12.4489975},{-2.7240696}},
//{{1.4165162},{16.780268},{8.471855999999999}},
//{{0.42223078},{-8.267551},{-7.3549875999999985}},
//{{-3.5685309999999997},{10.95076},{-0.8036005}},
//{{-4.671779},{11.727119},{0.38136974}},
//{{-2.1383946},{1.6889231},{3.5276701}},
//{{-1.334794},{2.4925237},{-0.3405087}},
//{{-2.9147543999999996},{19.57244},{7.586533500000001}},
//{{3.5276701},{-3.9499009},{-1.9204689999999998}},
//{{-4.0588636},{10.038197},{14.2877445}},
//{{7.586533500000001},{13.334320000000002},{3.8681788}},
//{{-5.175732},{14.1787815},{5.5162406}},
//{{1.1849703},{2.1111538},{2.2201166000000003}},
//{{-3.7183550000000003},{19.463476},{-0.8036005}},
//{{3.173541},{6.5105260000000005},{4.2086873}},
//{{-1.7297841},{-5.625203599999999},{-4.3312707}},
//{{0.84446156},{12.980191},{11.563675}},
//{{2.7649305},{17.352324},{7.7363577}},
//{{2.3426998},{15.894946},{3.6774937999999993}},
//{{4.7943625},{3.8273177000000005},{0.9942854}},
//{{-1.8387468999999996},{12.830366999999999},{-1.56634}},
//{{5.366417},{14.410328},{6.742072}},
//{{2.1111538},{0.27240697},{2.6423476000000004}},
//{{4.2086873},{11.454713},{7.6273947}},
//{{-5.1348709999999995},{9.00305},{-3.173541}},
//{{3.0237172},{7.164303},{0.08172209}},
//{{2.070293},{3.4868089999999996},{1.7570249}},
//{{-3.1054392},{4.862464},{-0.040861044}},
//{{-4.3993720000000005},{19.531578},{5.0531489999999994}},
//{{4.7535014},{-3.2961242},{-2.1383946}},
//{{-0.6946376999999999},{-7.6273947},{-7.8180795}},
//{{-2.8330324},{6.5513873},{-7.9679036}},
//{{-0.8036005},{10.460427000000001},{0.23154591}},
//{{-3.214402},{3.0645783},{3.2552630000000002}},
//{{-2.8738932999999998},{3.5957718},{0.10896278}},
//{{-4.4810944},{19.504337},{5.9793324}},
//{{2.7921712000000003},{-1.7570249},{0.19068487}},
//{{-4.7535014},{4.2086873},{11.141444}},
//{{-1.56634},{3.7592160000000003},{3.2552630000000002}},
//{{-3.214402},{2.9556155},{0.7627395}},
//{{-2.6423476000000004},{19.57244},{8.008764}},
//{{0.5720546},{-1.4982381999999999},{-0.38136974}},
//{{-1.525479},{-7.7363577},{-8.580819}},
//{{-4.7535014},{18.537292},{-3.1054392}},
//{{1.7978859},{14.410328},{1.0760075}},
//{{-0.8036005},{3.2552630000000002},{3.990762}},
//{{-1.2258313},{3.990762},{0.8036005}},
//{{-4.630918},{19.57244},{6.3607025}},
//{{1.9885708},{-1.3756552},{-0.5720546}},
//{{-4.630918},{1.9204689999999998},{8.281172}},
//{{12.299174},{17.883516},{5.325556}},
//{{0.9942854},{15.241169000000001},{5.325556}},
//{{1.0351465},{1.7570249},{1.9885708}},
//{{-9.384419},{19.57244},{0.10896278}},
//{{2.9556155},{5.597963},{4.630918}},
//{{-2.3699405},{-5.366417},{-2.7513103}},
//{{-1.8796080000000002},{10.882658000000001},{-7.7772183}}};
	
	/*
	Sitting:
	*/
//	ai_float in_data [90][3][1] = {
//			{{8.96},{-1.12},{2.26}},
//		{{8.88},{-1.08},{2.34}},
//		{{8.92},{-1.08},{2.26}},
//		{{8.92},{-1.08},{2.3}},
//		{{8.92},{-1.08},{2.22}},
//		{{8.96},{-1.08},{2.3}},
//		{{8.92},{-1.12},{2.26}},
//		{{8.88},{-1.08},{2.22}},
//		{{8.96},{-1.08},{2.3}},
//		{{8.96},{-1.08},{2.26}},
//		{{8.92},{-1.12},{2.18}},
//		{{8.96},{-1.08},{2.26}},
//		{{8.96},{-1.04},{2.22}},
//		{{8.96},{-1.08},{2.3}},
//		{{8.96},{-1.04},{2.18}},
//		{{8.96},{-1.04},{2.22}},
//		{{8.96},{-1.12},{2.26}},
//		{{8.85},{-1.08},{2.18}},
//		{{8.85},{-0.99},{2.26}},
//		{{8.85},{-1.04},{2.03}},
//		{{8.73},{-0.99},{1.88}},
//		{{9.04},{-1.12},{1.99}},
//		{{9.08},{-0.99},{2.07}},
//		{{10.08},{-1.65},{1.99}},
//		{{10.57},{-2.07},{2.07}},
//		{{8.12},{1.38},{1.69}},
//		{{6.4},{-0.15},{0.76}},
//		{{10.88},{-0.95},{3.34}},
//		{{10.5},{0.38},{2.53}},
//		{{8.73},{-0.61},{0.89}},
//		{{8.47},{-1.18},{1.12}},
//		{{8.43},{-1.46},{2.03}},
//		{{9.66},{-1.14},{1.46}},
//		{{9.43},{-1.14},{1.69}},
//		{{9.0},{-0.89},{1.65}},
//		{{8.77},{-0.8},{1.27}},
//		{{9.38},{-0.8},{1.88}},
//		{{9.19},{-0.8},{1.88}},
//		{{8.88},{-0.99},{1.53}},
//		{{8.96},{-0.95},{1.57}},
//		{{9.04},{-0.95},{1.76}},
//		{{8.96},{-0.99},{1.73}},
//		{{8.92},{-0.99},{1.69}},
//		{{9.04},{-0.95},{1.61}},
//		{{9.04},{-0.95},{1.65}},
//		{{9.08},{-0.95},{1.61}},
//		{{9.08},{-0.95},{1.76}},
//		{{9.0},{-0.99},{1.61}},
//		{{8.96},{-1.08},{1.69}},
//		{{9.0},{-1.08},{1.65}},
//		{{9.04},{-0.99},{1.61}},
//		{{9.0},{-0.95},{1.69}},
//		{{9.04},{-1.04},{1.69}},
//		{{9.04},{-1.04},{1.69}},
//		{{9.04},{-1.04},{1.69}},
//		{{9.0},{-0.99},{1.65}},
//		{{9.04},{-0.99},{1.69}},
//		{{9.11},{-1.08},{1.69}},
//		{{9.08},{-0.89},{1.65}},
//		{{9.08},{-0.91},{1.65}},
//		{{9.0},{-0.89},{1.84}},
//		{{8.92},{-1.04},{1.8}},
//		{{9.0},{-1.04},{1.88}},
//		{{9.3},{-1.14},{1.88}},
//		{{8.66},{-1.38},{1.38}},
//		{{8.73},{-1.73},{1.5}},
//		{{9.04},{-1.57},{1.57}},
//		{{9.72},{-0.84},{1.53}},
//		{{9.0},{-0.34},{1.88}},
//		{{8.39},{-1.18},{1.76}},
//		{{9.3},{-0.69},{2.07}},
//		{{8.27},{-1.33},{1.73}},
//		{{7.74},{-1.31},{1.27}},
//		{{8.81},{-1.31},{2.68}},
//		{{8.12},{-1.18},{2.45}},
//		{{8.85},{-1.33},{1.38}},
//		{{10.0},{-0.69},{1.61}},
//		{{10.0},{-1.8},{1.57}},
//		{{10.08},{-1.12},{1.46}},
//		{{8.31},{-1.38},{1.42}},
//		{{9.08},{-1.42},{1.31}},
//		{{8.81},{-1.61},{1.31}},
//		{{9.04},{-1.38},{1.12}},
//		{{9.34},{-1.38},{1.14}},
//		{{9.23},{-1.38},{1.08}},
//		{{8.73},{-1.8},{0.89}},
//		{{8.81},{-1.65},{0.91}},
//		{{9.34},{-1.14},{1.53}},
//		{{9.08},{-1.08},{1.18}},
//		{{8.88},{-1.38},{1.31}}};
	/*
	Standing:
	*/
//		ai_float in_data [90][3][1] = {
//		{{3.26},{9.3},{1.1441092}},
//{{3.17},{9.34},{1.1168685}},
//{{3.21},{9.3},{1.1168685}},
//{{3.21},{9.28},{1.1168685}},
//{{3.26},{9.28},{1.1849703}},
//{{3.17},{9.3},{1.1168685}},
//{{3.21},{9.28},{1.1168685}},
//{{-1.88},{9.85},{-0.23154591}},
//{{-0.19},{9.92},{-0.5720546}},
//{{-0.61},{10.27},{-0.88532263}},
//{{0.76},{10.57},{-1.7570249}},
//{{0.42},{9.47},{-1.1168685}},
//{{0.38},{9.47},{-1.9477096999999999}},
//{{-1.04},{10.65},{-1.525479}},
//{{-1.92},{9.51},{-0.5720546}},
//{{-1.31},{9.85},{-0.53119355}},
//{{-0.08},{9.92},{-1.7570249}},
//{{1.73},{9.77},{-0.8036005}},
//{{1.5},{9.92},{-1.4573771999999998}},
//{{1.27},{10.5},{-1.8796080000000002}},
//{{0.61},{10.12},{-1.9885708}},
//{{-0.53},{9.77},{-1.8796080000000002}},
//{{-0.42},{9.62},{-1.6480621}},
//{{0.65},{10.42},{-2.2201166000000003}},
//{{0.65},{10.42},{-2.2201166000000003}},
//{{1.61},{9.38},{-1.8387468999999996}},
//{{1.61},{9.43},{-1.525479}},
//{{1.61},{9.43},{-1.525479}},
//{{0.95},{10.27},{-1.3075534}},
//{{0.19},{10.38},{-1.1849703}},
//{{0.31},{9.81},{-1.4165162}},
//{{1.12},{9.62},{-1.6889231}},
//{{1.23},{9.85},{-1.6480621}},
//{{1.04},{9.7},{-1.8387468999999996}},
//{{0.57},{9.89},{-2.0294318}},
//{{0.65},{9.96},{-1.9885708}},
//{{0.95},{9.96},{-1.7570249}},
//{{1.42},{10.0},{-1.7297841}},
//{{1.69},{9.89},{-1.525479}},
//{{1.46},{10.0},{-1.4165162}},
//{{0.69},{9.77},{-1.6889231}},
//{{0.08},{9.96},{-1.9477096999999999}},
//{{-0.08},{10.19},{-2.1111538}},
//{{0.38},{9.72},{-1.9885708}},
//{{0.93},{10.12},{-2.1111538}},
//{{1.33},{9.62},{-1.9885708}},
//{{1.08},{9.85},{-1.9477096999999999}},
//{{0.8},{9.77},{-1.7570249}},
//{{0.69},{10.34},{-1.6889231}},
//{{0.72},{9.66},{-1.3075534}},
//{{0.69},{10.0},{-1.3756552}},
//{{0.93},{9.62},{-1.4573771999999998}},
//{{0.76},{10.12},{-1.607201}},
//{{0.93},{9.72},{-1.7978859}},
//{{0.76},{10.23},{-1.9885708}},
//{{0.76},{9.23},{-1.9204689999999998}},
//{{0.57},{10.34},{-2.1383946}},
//{{0.99},{9.58},{-1.8796080000000002}},
//{{1.33},{10.04},{-1.7978859}},
//{{1.61},{9.85},{-1.4165162}},
//{{0.61},{10.15},{-0.88532263}},
//{{0.53},{9.58},{-1.4573771999999998}},
//{{0.15},{10.19},{-1.9204689999999998}},
//{{0.34},{9.85},{-1.334794}},
//{{0.8},{10.31},{-1.7978859}},
//{{0.69},{9.53},{-1.9477096999999999}},
//{{0.8},{9.92},{-1.8796080000000002}},
//{{0.5},{10.04},{-1.1849703}},
//{{1.12},{9.43},{-1.7978859}},
//{{1.31},{10.27},{-1.2666923}},
//{{1.5},{9.77},{-1.607201}},
//{{0.46},{10.04},{-0.9125633}},
//{{0.31},{9.85},{-1.0760075}},
//{{0.61},{10.19},{-1.1849703}},
//{{0.72},{9.81},{-1.1849703}},
//{{1.04},{9.92},{-1.9885708}},
//{{0.99},{9.77},{-1.7297841}},
//{{0.76},{10.04},{-1.7978859}},
//{{0.69},{9.81},{-1.4982381999999999}},
//{{0.8},{10.27},{-1.2258313}},
//{{0.8},{9.43},{-1.4573771999999998}},
//{{1.04},{9.92},{-1.3075534}},
//{{1.04},{10.19},{-1.607201}},
//{{0.53},{9.89},{-1.6889231}},
//{{0.31},{9.92},{-1.7570249}},
//{{0.95},{9.72},{-1.6480621}},
//{{1.27},{10.04},{-1.7297841}},
//{{1.18},{9.72},{-1.525479}},
//{{0.84},{10.08},{-1.4573771999999998}},
//{{0.76},{9.77},{-1.3756552}}};
/*
	Upstairs:
	*/
//	ai_float in_data [90][3][1] = {
//				{{-1.56634},{13.252598999999998},{2.2201166000000003}},
//		{{-3.4459480000000005},{13.756551000000002},{2.4516625}},
//		{{-4.2495484},{9.847511},{2.3699405}},
//		{{-1.0760075},{7.7772183},{1.6889231}},
//		{{-1.0351465},{2.7649305},{2.5333846}},
//		{{-3.5957718},{6.3198414000000005},{11.114203}},
//		{{-4.2086873},{4.2086873},{2.2609777}},
//		{{-0.6946376999999999},{6.742072},{1.4982381999999999}},
//		{{-1.2258313},{9.384419},{0.61291564}},
//		{{0.08172209},{7.6273947},{2.7649305}},
//		{{-0.95342433},{14.1787815},{2.7649305}},
//		{{-2.7240696},{14.09706},{2.9556155}},
//		{{-4.7943625},{7.927042500000001},{0.3405087}},
//		{{-1.3075534},{4.862464},{0.27240697}},
//		{{-1.4165162},{0.38136974},{2.6423476000000004}},
//		{{-3.214402},{6.0882955},{12.026767}},
//		{{-4.290409599999999},{9.724928},{2.0294318}},
//		{{-3.2552630000000002},{7.436710000000001},{1.3075534}},
//		{{0.19068487},{8.471855999999999},{-0.88532263}},
//		{{-0.08172209},{8.962189},{-0.9942854}},
//		{{-0.46309182},{9.765789},{1.3756552}},
//		{{-5.0531489999999994},{11.223166},{-0.23154591}},
//		{{-7.8861814},{12.789507},{-1.8796080000000002}},
//		{{1.8387468999999996},{9.084772000000001},{1.9204689999999998}},
//		{{0.14982383},{3.8273177000000005},{1.9204689999999998}},
//		{{-2.4108016},{9.888372},{7.927042500000001}},
//		{{-6.5922484},{14.2877445},{-0.9942854}},
//		{{-0.88532263},{17.543009},{-0.38136974}},
//		{{-0.53119355},{12.299174},{0.08172209}},
//		{{-0.10896278},{8.240311},{0.3405087}},
//		{{-0.50395286},{10.800936},{1.4982381999999999}},
//		{{-3.1463002999999996},{14.941522},{0.42223078}},
//		{{-1.1849703},{11.304889},{1.2258313}},
//		{{-5.325556},{8.689782000000001},{0.53119355}},
//		{{-0.84446156},{6.4015636},{1.8387468999999996}},
//		{{-0.88532263},{2.6014863999999998},{3.4868089999999996}},
//		{{-4.2495484},{5.5571017},{4.440233}},
//		{{-4.7535014},{7.014479},{1.9204689999999998}},
//		{{-0.3405087},{9.425281},{2.1111538}},
//		{{-1.56634},{8.812365},{1.1441092}},
//		{{-1.8796080000000002},{7.2732660000000005},{1.6889231}},
//		{{-0.23154591},{11.264028},{3.7864565999999997}},
//		{{-3.5276701},{14.437569},{1.2666923}},
//		{{-1.2258313},{10.460427000000001},{1.6889231}},
//		{{-1.2666923},{9.697687},{2.4516625}},
//		{{-0.38136974},{4.3993720000000005},{3.5276701}},
//		{{-2.5333846},{3.4868089999999996},{6.5513873}},
//		{{-5.012288},{7.3141264999999995},{4.2495484}},
//		{{-1.3756552},{9.888372},{2.070293}},
//		{{-1.56634},{7.2732660000000005},{0.6946376999999999}},
//		{{-1.1441092},{11.958665},{4.140586}},
//		{{-7.1234417},{14.941522},{1.56634}},
//		{{-3.1463002999999996},{10.56939},{2.3018389}},
//		{{-0.8036005},{8.771504},{2.8330324}},
//		{{-0.95342433},{3.4459480000000005},{2.7240696}},
//		{{-1.3756552},{2.2609777},{7.9679036}},
//		{{-4.5628166},{12.830366999999999},{3.990762}},
//		{{-0.5720546},{12.108489},{2.4516625}},
//		{{-1.9204689999999998},{7.436710000000001},{1.334794}},
//		{{-3.0645783},{7.014479},{1.8796080000000002}},
//		{{0.040861044},{11.413852},{3.4868089999999996}},
//		{{-0.95342433},{11.454713},{1.4165162}},
//		{{-2.4516625},{10.501288},{1.7978859}},
//		{{-2.3699405},{9.275456},{2.9556155}},
//		{{-1.4982381999999999},{4.7943625},{2.9556155}},
//		{{-0.27240697},{3.0237172},{8.349273}},
//		{{-4.8216033},{10.991621},{4.3993720000000005}},
//		{{-1.2666923},{9.656827},{2.3018389}},
//		{{-1.7978859},{7.927042500000001},{1.56634}},
//		{{-1.9204689999999998},{7.3549875999999985},{2.152015}},
//		{{-1.1849703},{10.760075},{4.290409599999999}},
//		{{-1.4573771999999998},{16.276316},{3.9499009}},
//		{{-1.334794},{10.038197},{1.4982381999999999}},
//		{{-4.372132},{7.6273947},{0.313268}},
//		{{-2.5333846},{4.3312707},{2.0294318}},
//		{{-1.3075534},{2.3426998},{5.325556}},
//		{{-5.175732},{12.489859},{4.944186}},
//		{{-1.9477096999999999},{13.252598999999998},{2.8738932999999998}},
//		{{-0.6946376999999999},{8.19945},{1.4982381999999999}},
//		{{-1.334794},{5.366417},{1.6480621}},
//		{{-1.4573771999999998},{15.213928},{4.5219555}},
//		{{-3.5276701},{12.108489},{2.1792557}},
//		{{-2.2609777},{10.460427000000001},{1.3075534}},
//		{{-5.7886477},{7.3141264999999995},{-0.08172209}},
//		{{-1.525479},{4.4810944},{1.4165162}},
//		{{-2.0294318},{1.4573771999999998},{2.6423476000000004}},
//		{{-5.8158884},{7.504811999999999},{2.7649305}},
//		{{-2.982856},{6.3607025},{1.3075534}},
//		{{-0.7627395},{8.771504},{1.6480621}},
//		{{-0.42223078},{9.275456},{0.27240697}}};
	/*
	Walking:
	*/
	
	ai_float in_data [90][3][1] = {
		{{-4.099725},{12.830366999999999},{-3.6774937999999993}},
		{{1.4982381999999999},{14.750835999999998},{-2.6423476000000004}},
		{{0.8036005},{8.008764},{-3.1054392}},
		{{0.84446156},{10.882658000000001},{1.607201}},
		{{0.84446156},{15.431854000000001},{0.5720546}},
		{{-7.8180795},{14.137920999999999},{-2.8738932999999998}},
		{{4.440233},{5.175732},{-0.14982383}},
		{{-2.070293},{1.4573771999999998},{-1.4573771999999998}},
		{{-2.5333846},{19.19107},{15.132206}},
		{{-1.3756552},{5.012288},{0.7218784}},
		{{-1.4573771999999998},{6.5922484},{2.3699405}},
		{{-0.3405087},{7.586533500000001},{0.6946376999999999}},
		{{0.3405087},{12.176591},{0.84446156}},
		{{-0.040861044},{13.633968},{-1.0760075}},
		{{-6.0474343},{13.375181},{1.4573771999999998}},
		{{-0.3405087},{2.7649305},{-1.1168685}},
		{{-3.8681788},{6.633109599999999},{5.2165932999999995}},
		{{-5.366417},{3.336985},{-6.428804}},
		{{-0.29964766},{3.6366330000000002},{-3.0645783}},
		{{0.92618364},{6.66035},{-2.4108016}},
		{{-2.0294318},{10.310603},{1.9477096999999999}},
		{{1.5390993},{14.1787815},{-0.040861044}},
		{{-4.5628166},{13.293459},{-4.140586}},
		{{5.597963},{8.662541000000001},{0.313268}},
		{{-1.7297841},{1.6889231},{-0.38136974}},
		{{-4.2086873},{16.08563},{13.102775}},
		{{-6.66035},{6.4424243},{2.8330324}},
		{{-0.08172209},{8.008764},{1.9204689999999998}},
		{{-0.14982383},{9.507003},{0.14982383}},
		{{1.334794},{9.656827},{-0.7218784}},
		{{-0.3405087},{15.704260999999999},{0.8036005}},
		{{-8.62168},{15.785982999999998},{0.313268}},
		{{3.0645783},{4.944186},{-0.23154591}},
		{{-0.9942854},{1.1168685},{-0.95342433}},
		{{-0.5720546},{14.669114000000002},{11.645397}},
		{{2.152015},{6.5513873},{-2.4925237}},
		{{-1.334794},{12.598822},{1.7297841}},
		{{0.14982383},{7.8861814},{-1.4165162}},
		{{2.8738932999999998},{12.830366999999999},{0.19068487}},
		{{-0.46309182},{13.021052000000001},{0.5720546}},
		{{-7.504811999999999},{16.317176999999997},{3.0645783}},
		{{-0.0},{3.2961242},{-1.0351465}},
		{{-3.0645783},{4.5219555},{1.9885708}},
		{{-1.6889231},{2.6014863999999998},{-1.4982381999999999}},
		{{1.334794},{3.9090397000000006},{-4.1814466}},
		{{-1.7978859},{5.9793324},{-1.2258313}},
		{{-0.6537767},{9.997335000000001},{1.4165162}},
		{{-0.3405087},{14.709975},{0.5720546}},
		{{-2.9556155},{13.443283},{-4.3312707}},
		{{5.7477865},{7.5456724},{-0.50395286}},
		{{-2.5333846},{1.8796080000000002},{-0.9942854}},
		{{-3.6774937999999993},{16.467001},{11.495573}},
		{{-6.469665},{17.2025},{1.7297841}},
		{{-1.1441092},{12.067628},{0.313268}},
		{{0.38136974},{8.430995},{-1.4165162}},
		{{-0.08172209},{10.691972999999999},{0.7218784}},
		{{-0.42223078},{14.982382999999999},{0.3405087}},
		{{-7.7772183},{15.132206},{-0.27240697}},
		{{2.4925237},{9.888372},{0.19068487}},
		{{-0.8036005},{2.7649305},{-1.1168685}},
		{{-3.7864565999999997},{9.80665},{4.903325}},
		{{-3.5957718},{8.158588},{-4.099725}},
		{{0.040861044},{7.586533500000001},{-2.3018389}},
		{{-1.56634},{6.3198414000000005},{-2.3699405}},
		{{0.61291564},{10.460427000000001},{0.46309182}},
		{{0.95342433},{14.818938000000001},{0.95342433}},
		{{-5.243834},{14.47843},{-3.1463002999999996}},
		{{4.2495484},{6.782933},{-0.42223078}},
		{{-2.4516625},{2.1111538},{-1.4165162}},
		{{-2.070293},{18.496431},{13.633968}},
		{{-5.09401},{14.709975},{2.6832085}},
		{{-0.0},{9.384419},{0.6537767}},
		{{-2.3018389},{9.152874},{1.5390993}},
		{{2.6423476000000004},{10.419566},{0.3405087}},
		{{0.46309182},{12.9529505},{-0.61291564}},
		{{-6.5513873},{13.674829500000001},{0.8036005}},
		{{0.14982383},{2.6014863999999998},{-1.4982381999999999}},
		{{-4.290409599999999},{5.5162406},{2.5606253}},
		{{-5.47538},{10.841797},{-2.7240696}},
		{{2.4516625},{14.709975},{-0.3405087}},
		{{1.4573771999999998},{8.471855999999999},{-2.8330324}},
		{{0.6946376999999999},{11.141444},{0.8036005}},
		{{2.1111538},{15.132206},{0.42223078}},
		{{-4.5219555},{13.0619135},{-3.7183550000000003}},
		{{4.0588636},{9.152874},{-0.50395286}},
		{{-2.3426998},{1.9204689999999998},{-1.7570249}},
		{{-3.7183550000000003},{14.369467000000002},{12.367275999999999}},
		{{-5.434519},{13.443283},{0.27240697}},
		{{-0.5720546},{10.923519},{2.2201166000000003}},
		{{0.92618364},{10.460427000000001},{-0.42223078}}};
	aiRun(in_data,out_data);
//	printf("in_data [%f %f %f]\r\n",in_data[0],in_data[1],in_data[2]);
	printf("Confidence\r\nDownstairs:%.2f\r\nJogging:%.2f\r\nSitting:%.2f\r\nStanding:%.2f\r\nUpstairs:%.2f\r\nWalking:%.2f\r\n",out_data[0]*100,out_data[1]*100,out_data[2]*100,out_data[3]*100,out_data[4]*100,out_data[5]*100);
	if(out_data[0]>0.5f){
		printf("This man is go Downstairs!\r\n");
	}
	else if (out_data[1]>0.5f){
		printf("This man is Jogging!\r\n");
	}
	else if (out_data[2]>0.5f){
		printf("This man is Sitting!\r\n");
	}
	else if (out_data[3]>0.5f){
		printf("This man is Standing!\r\n");
	}
	else if (out_data[4]>0.5f){
		printf("This man is go Upstairs!\r\n");
	}
	else if (out_data[5]>0.5f){
		printf("This man is Walking!\r\n");
	}

	while(1);
	
    /* USER CODE END 1 */
}
#ifdef __cplusplus
}
#endif
