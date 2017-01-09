#include "common.h"
#include "string.h"

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

typedef struct {
	bool opened;
	uint32_t offset;
} Fstate;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

static Fstate file[NR_FILES + 3];

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);
void serial_printc(char);

/* TODO: implement a simplified file system here. */

static inline char *get_filename(int index) {

	assert(index >= 0 && index < NR_FILES);
	return file_table[index].name;
}

int fs_open(const char *pathname, int flags) {

	int index = 0;
	while(index < NR_FILES) {
		if(strcmp(get_filename(index),pathname) == 0) {//compare file name one by one
			file[index+3].opened = true;//open file
			file[index+3].offset = 0;
			break;
		}
		index++;
	}
	return (index < NR_FILES)?(index + 3):-1;	

}

int fs_read(int fd ,void *buf,size_t len) {

	uint32_t end = len + file[fd].offset;
	uint32_t start = file_table[fd-3].disk_offset + file[fd].offset;

	if(end <= file_table[fd-3].size) {//not across the file size
		file[fd].offset += len;	
	}
	else {//across the file size
		len = file_table[fd-3].size - file[fd].offset;
		file[fd].offset = file_table[fd-3].size;	
	}

	ide_read(buf,start,len);//read file
	return len;
}

int fs_write(int fd, void* buf, size_t len) {

	switch(fd) {
		case 1:case 2: {// system error and system write
			int i;	
			for(i = 0; i < len; i++)
				serial_printc(*(char *)(buf+i));
			break;
		}
	
		default: {//normal file	
			ide_write((void *)file_table[fd-3].disk_offset + file[fd].offset,(uint32_t)buf,len);
			file[fd].offset += len;
		}
	}

	return len;
}

int fs_lseek(int fd, int offset, int whence) {

	int ret = -1;

	switch(whence) {
		
		case SEEK_SET: {
			ret = file[fd].offset = offset;
			break;
		}
		case SEEK_CUR: {
			file[fd].offset += offset;
			ret = file[fd].offset + offset;
			break;
		}
		case SEEK_END: {
		        ret = file[fd].offset = file_table[fd-3].size + offset;
			break;
		}
		default:break;
	}

	return ret;
}

void fs_close(int fd) {//close file

	file[fd].offset = 0;
	file[fd].opened = false;
}



