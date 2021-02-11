typedef struct agent_arg agent_arg_t;
static int sim_agent_queue_request(agent_arg_t *agent_arg_ptr);

#include "src/slurmctld/agent.c"
#include "src/common/sim/sim.h"

extern void sim_complete_job(uint32_t job_id);

static int sim_agent_queue_request(agent_arg_t *agent_arg_ptr)
{
	kill_job_msg_t * kill_job;
	batch_job_launch_msg_t *launch_msg_ptr;
//	job_record_t *job_ptr;
//	time_t now;
	//queued_request_t *queued_req_ptr = NULL;
	//__real_agent_queue_request(agent_arg_ptr);
	//return;

	debug("Sim: __wrap_agent_queue_request msg_type=%s", rpc_num2string(agent_arg_ptr->msg_type));
	//__real_agent_queue_request(agent_arg_ptr);

	switch(agent_arg_ptr->msg_type) {
	case REQUEST_BATCH_JOB_LAUNCH:
		launch_msg_ptr = (batch_job_launch_msg_t *)agent_arg_ptr->msg_args;
		sim_insert_event_comp_job(launch_msg_ptr->job_id);
		return 1;
	case REQUEST_KILL_TIMELIMIT:
		kill_job = (kill_job_msg_t*)agent_arg_ptr->msg_args;
		//complete_job(kill_job->job_id);
		sim_job_requested_kill_timelimit(kill_job->job_id);
		return 1;
	case REQUEST_TERMINATE_JOB:
		// this initiated from job_compleate by jobs finishing by themselves
		return 1;
	case REQUEST_NODE_REGISTRATION_STATUS:
		debug("Sim: __wrap_agent_queue_request msg_type=%s", rpc_num2string(agent_arg_ptr->msg_type));
		return 1;
	case REQUEST_PARTITION_INFO:
		return 0;
	case REQUEST_NODE_INFO:
		return 0;
	case REQUEST_SUBMIT_BATCH_JOB:
		return 0;
	default:
		error("Sim: unknown for sim request will use normal slurm (msg_type=%s)", rpc_num2string(agent_arg_ptr->msg_type));
		break;
	}
	return 0;
}
