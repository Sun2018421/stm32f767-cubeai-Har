
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
	ai_float in_data [90][3][1] = {
{{11.604536},{17.079916},{1.334794}},
{{-3.173541},{14.015338},{5.706926}},
{{0.61291564},{1.1168685},{2.5606253}},
{{-7.8861814},{19.57244},{1.9885708}},
{{3.1463002999999996},{5.243834},{4.671779}},
{{-3.0237172},{-4.3312707},{-3.336985}},
{{-0.08172209},{11.917805},{-7.8861814}},
{{-1.0351465},{14.818938000000001},{4.6036777}},
{{-2.4516625},{2.5333846},{3.4868089999999996}},
{{-1.3756552},{2.070293},{-0.19068487}},
{{-2.4925237},{19.57244},{6.469665}},
{{1.4573771999999998},{-5.243834},{-4.372132}},
{{-1.4165162},{9.80665},{5.7477865}},
{{-1.2666923},{14.709975},{6.2108784}},
{{-3.6774937999999993},{3.173541},{3.7864565999999997}},
{{1.8387468999999996},{2.7649305},{-1.7570249}},
{{-1.2666923},{19.313654},{6.3198414000000005}},
{{2.4108016},{-7.654635400000001},{-6.1291566}},
{{-0.61291564},{16.358038},{4.944186}},
{{0.040861044},{17.502148000000002},{2.5333846}},
{{-7.654635400000001},{7.8180795},{4.372132}},
{{-1.2666923},{0.7218784},{0.8036005}},
{{-5.012288},{19.57244},{5.5162406}},
{{1.9477096999999999},{2.7921712000000003},{2.070293}},
{{-5.0531489999999994},{1.6480621},{7.6273947}},
{{9.384419},{13.443283},{1.0351465}},
{{-5.434519},{13.211738},{6.4424243}},
{{-0.61291564},{1.8796080000000002},{1.4165162}},
{{4.7126402999999994},{-6.5513873},{-6.0201936}},
{{-1.7570249},{9.302697},{-6.428804}},
{{-0.9125633},{10.501288},{-0.27240697}},
{{2.6014863999999998},{19.381754},{4.440233}},
{{5.7886477},{3.214402},{1.1441092}},
{{-1.9885708},{12.4489975},{-2.7240696}},
{{1.4165162},{16.780268},{8.471855999999999}},
{{0.42223078},{-8.267551},{-7.3549875999999985}},
{{-3.5685309999999997},{10.95076},{-0.8036005}},
{{-4.671779},{11.727119},{0.38136974}},
{{-2.1383946},{1.6889231},{3.5276701}},
{{-1.334794},{2.4925237},{-0.3405087}},
{{-2.9147543999999996},{19.57244},{7.586533500000001}},
{{3.5276701},{-3.9499009},{-1.9204689999999998}},
{{-4.0588636},{10.038197},{14.2877445}},
{{7.586533500000001},{13.334320000000002},{3.8681788}},
{{-5.175732},{14.1787815},{5.5162406}},
{{1.1849703},{2.1111538},{2.2201166000000003}},
{{-3.7183550000000003},{19.463476},{-0.8036005}},
{{3.173541},{6.5105260000000005},{4.2086873}},
{{-1.7297841},{-5.625203599999999},{-4.3312707}},
{{0.84446156},{12.980191},{11.563675}},
{{2.7649305},{17.352324},{7.7363577}},
{{2.3426998},{15.894946},{3.6774937999999993}},
{{4.7943625},{3.8273177000000005},{0.9942854}},
{{-1.8387468999999996},{12.830366999999999},{-1.56634}},
{{5.366417},{14.410328},{6.742072}},
{{2.1111538},{0.27240697},{2.6423476000000004}},
{{4.2086873},{11.454713},{7.6273947}},
{{-5.1348709999999995},{9.00305},{-3.173541}},
{{3.0237172},{7.164303},{0.08172209}},
{{2.070293},{3.4868089999999996},{1.7570249}},
{{-3.1054392},{4.862464},{-0.040861044}},
{{-4.3993720000000005},{19.531578},{5.0531489999999994}},
{{4.7535014},{-3.2961242},{-2.1383946}},
{{-0.6946376999999999},{-7.6273947},{-7.8180795}},
{{-2.8330324},{6.5513873},{-7.9679036}},
{{-0.8036005},{10.460427000000001},{0.23154591}},
{{-3.214402},{3.0645783},{3.2552630000000002}},
{{-2.8738932999999998},{3.5957718},{0.10896278}},
{{-4.4810944},{19.504337},{5.9793324}},
{{2.7921712000000003},{-1.7570249},{0.19068487}},
{{-4.7535014},{4.2086873},{11.141444}},
{{-1.56634},{3.7592160000000003},{3.2552630000000002}},
{{-3.214402},{2.9556155},{0.7627395}},
{{-2.6423476000000004},{19.57244},{8.008764}},
{{0.5720546},{-1.4982381999999999},{-0.38136974}},
{{-1.525479},{-7.7363577},{-8.580819}},
{{-4.7535014},{18.537292},{-3.1054392}},
{{1.7978859},{14.410328},{1.0760075}},
{{-0.8036005},{3.2552630000000002},{3.990762}},
{{-1.2258313},{3.990762},{0.8036005}},
{{-4.630918},{19.57244},{6.3607025}},
{{1.9885708},{-1.3756552},{-0.5720546}},
{{-4.630918},{1.9204689999999998},{8.281172}},
{{12.299174},{17.883516},{5.325556}},
{{0.9942854},{15.241169000000001},{5.325556}},
{{1.0351465},{1.7570249},{1.9885708}},
{{-9.384419},{19.57244},{0.10896278}},
{{2.9556155},{5.597963},{4.630918}},
{{-2.3699405},{-5.366417},{-2.7513103}},
{{-1.8796080000000002},{10.882658000000001},{-7.7772183}}};

	aiRun(in_data,out_data);
//	printf("in_data [%f %f %f]\r\n",in_data[0],in_data[1],in_data[2]);
	printf("out_data [%f %f %f %f %f %f]\r\n",out_data[0],out_data[1],out_data[2],out_data[3],out_data[4],out_data[5]);

		while(1);
	
    /* USER CODE END 1 */
}
#ifdef __cplusplus
}
#endif
