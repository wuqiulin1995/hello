#include <stdio.h>
#include <liblightnvm.h>

int main(int argc, char **argv)
{
	struct nvm_dev *dev = nvm_dev_open("/dev/nvme0n1");
	if (!dev) {
		perror("nvm_dev_open");
		return 1;
	}
	nvm_dev_pr(dev);

	const struct nvm_geo *geo=nvm_dev_get_geo(dev);
	nvm_geo_pr(geo);
	
	char *buf_w_d=nvm_buf_alloc(geo,16384);   //16K buffer
	nvm_buf_fill(buf_w_d,16384);
	char *buf_w_m=nvm_buf_alloc(geo,16);
	nvm_buf_fill(buf_w_m,16);
	nvm_buf_to_file(buf_w_d,16384,"buf_w_d");
	nvm_huf_to_file(buf_w_m,16,"buf_w_m")

	char *buf_r_d=nvm_buf_alloc(geo,16384);   //16K buffer
	char *buf_r_m=nvm_buf_alloc(geo,16);

	struct nvm_addr addr[4];
	
	for(int i=0;i<4;i++)
	{
	    addr[i]=nvm_addr_dev2gen(dev,0x0000000000005004+i);
	    nvm_addr_pr(addr[i]);
	}	

	struct nvm_ret *ret_w,*ret_r;

	if(nvm_addr_write(dev,addr,4,buf_w_d,buf_w_m,0,ret_w)==-1)
	{
	    printf("write error\n");
	}

	if(nvm_addr_read(dev,addr,4,buf_r_d,buf_r_m,0,ret_r)==-1)
	{
	    printf("read error\n");
	}
	else
	{
	    nvm_buf_to_file(buf_r_d,16384,"buf_r_d");
	    nvm_buf_to_file(buf_r_m,16,"buf_r_m");
	}

	nvm_dev_close(dev);
	return 0;
}
