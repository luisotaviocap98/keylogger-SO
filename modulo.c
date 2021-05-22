#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h> 
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Luis Otavio O.C, Pedro P.");
 
//  receber parametros
static char arquivo[BUF_LEN];
module_param_string(arquivo, arq, BUF_LEN, 0);


// sera passado um nome de arquivo
// pegar o ponteiro de arquivo
// descobrir inode dele
// pegar informaçoes do inode

static struct inode *get_info(struct inode *inode)
{
	umode_t modo = inode->i_mode; //permissoes
	uid_t user = inode->i_uid; //user id owner
	loff_t tam = inode->i_size; //tamanho arquivo
	struct timespec acesso = inode->i_atime; //ultimo acesso
 	struct inode_operations *iop = inode->i_op; //operacoes de inode
	struct file_operations *fop = inode->i_fop; //operacoes de arquivo 
	struct super_block *sblk = inode->i_sb; //superbloco
};

static struct inode *V1_minix_iget(struct inode *inode)
{
      struct buffer_head * bh;
      struct minix_inode * raw_inode;
      struct minix_inode_info *minix_inode = minix_i(inode);
      int i;

      raw_inode = minix_V1_raw_inode(inode->i_sb, inode->i_ino, &bh);
      if (!raw_inode) {
              iget_failed(inode);
              return ERR_PTR(-EIO);
}


static struct inode_operations Inode_Ops_4_Our_Proc_File = {
 .permission = module_permission, /* check for permissions */
};

static struct file_operations file_ops = {
 .read = procfs_read,
 .write = procfs_write,
 .open = procfs_open,
 .release = procfs_close,
};

static int __init simple_init(void)
{
	printk(KERN_ALERT "hello...\n");
	return 0;
}
 
static void __exit simple_cleanup(void)
{
	printk(KERN_WARNING "bye ...\n");
}
 
module_init(simple_init);
module_exit(simple_cleanup);