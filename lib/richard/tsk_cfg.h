/****************************************************************************
* Title                 :   Task Configuration
* Filename              :   tsk_50ms.c
* Author                :   JWB
* Origin Date           :   11/07/2012
* Version               :   1.0.0
* Compiler              :   IAR C/C++ Compiler for ARM v6.40.4
* Target                :   MKL25Z128VLK4
* Copyright             :   Beningo Engineering
* All Rights Reserved
*
* THIS SOFTWARE IS PROVIDED BY BENINGO ENGINEERING "AS IS" AND ANY EXPRESSED
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL BENINGO ENGINEERING OR ITS CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
* Notes                 :   None
*
* Revision Log:
*
* REV           Author        Date                  Changes
* 1.0.0          JWB       11/07/2012           Initial Release
*******************************************************************************/
/** \file tsk_cfg.c
 * \brief This is source file for the configuration of the application tasks.
 *  Including this file will include all tasks and can also be used to turn off
 *  tasks that will not be used in an application.
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "tsk.h"                   	// For continuous tasks
#include "tsk_10ms.h"             	// For 10 ms tasks
#include "tsk_20ms.h"             	// For 100 ms tasks

// #include "tsk_100ms.h"             	// For 100 ms tasks
#include "scheduler.h"				// For task interval definitions

/******************************************************************************
* Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Configuration
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variable Declarations
***************************************** **************************************/
/**
 * Task configuration table.  Holds the task interval, last time executed, and
 * the function to be executed.  A continuous task is defined as a task with
 * an interval of 0.  Last time executed is set to 0.
 */
static TaskType Tasks[] =
{
	{ 500 ,  0, tsk_10ms   },
  { 100,  0, tsk_20ms  },
};

/**
 * Defines the number of tasks that are currently scheduled to run.
 */
uint8_t Task_Number = sizeof(Tasks) / sizeof(*Tasks);

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function : Tsk_GetConfig()
*//**
* \section Description Description:
*  The Tsk_GetConfig function returns a pointer to the task configuration table.
*
* \param  None.
*
* \return TaskType * - Pointer to the Task configuration table
*
* \section Example Example:
* \code
*    static TaskType *Task_ptr;
*
*    Task_Ptr = Tsk_GetConfig();
* \endcode
*
* \see Tsk_GetNumTasks
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Software Version    Initials   Description
*  01/02/14       0.1.1            JWB      Function Created.
*
*******************************************************************************/
TaskType *Tsk_GetConfig(void)
{
   	return Tasks;
}

/******************************************************************************
* Function : Tsk_GetNumTasks()
*//**
* \section Description Description:
*  The Tsk_GetNumTasks function returns the number of tasks that are in the
*  task configuration table.
*
* \param  None.
*
* \return uint8_t - The number of tasks in the task configuration table.
*
* \section Example Example:
* \code
*    const uint8_t NumTasks = Tsk_GetNumTasks();
* \endcode
*
* \see Tsk_GetConfig
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Software Version    Initials   Description
*  01/02/14       0.1.1            JWB      Function Created.
*
*******************************************************************************/
uint8_t Tsk_GetNumTasks(void)
{
	return sizeof(Tasks) / sizeof(*Tasks);
}

/*** End of File **************************************************************/
