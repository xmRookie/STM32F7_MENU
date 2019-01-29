#include "FFmpegMux.h"

//Read File  
int read_video_buffer(void *opaque, uint8_t *buf, int buf_size){  
    //if(!feof(fp_open)){  
        //int true_size=fread(buf,1,buf_size,fp_open);  
     //   return true_size;  
    //}else{  
    //    return -1;  
    //}  
}  
  
//Write File  
int write_AV_buffer(void *opaque, uint8_t *buf, int buf_size){  
    //if(!feof(fp_write)){  
        //int true_size=fwrite(buf,1,buf_size,fp_write);  
     //   return true_size;  
    //}else{  
    //    return -1;  
    //}  
}  
  


bool Muxer(const char * szVideoInput, const char * szAudioInput, const char * szVideoOutput)
{
	AVOutputFormat *ofmt = NULL;
	//Input AVFormatContext and Output AVFormatContext  
	AVFormatContext *ifmt_ctx_v = NULL, *ifmt_ctx_a = NULL, *ofmt_ctx = NULL;
	AVPacket *tmp_pkt;

	AVCodecContext *video_dec_ctx = NULL, *audio_dec_ctx;

	AVStream *video_stream = NULL, *audio_stream = NULL;
	AVStream *i_video_stream;
	AVStream *o_video_stream;
	int ret;
	unsigned int i;
	int videoindex_v = -1, videoindex_out = -1;
	int audioindex_a = -1, audioindex_out = -1;
	int frame_index = 0;
	int64_t cur_pts_v = 0, cur_pts_a = 0, inter_pts_a = 0;
	int index_pts_a = 0;
	unsigned char* inbuffer_v=NULL;  
    unsigned char* outbuffer=NULL;  

	tmp_pkt = av_packet_alloc();//创建packet的时候，并没有给数据域分配空间 数据域的空间实在av_read_frame内分配的
	if (!tmp_pkt)
	{
		printf("Could not av_packet_alloc.");
	}

	//av_register_all();
	//Input  

	//ifmt_ctx_v = avformat_alloc_context();
    //ifmt_ctx_a = avformat_alloc_context();

	inbuffer_v=(unsigned char*)av_malloc(32768);  
    outbuffer=(unsigned char*)av_malloc(32768);  
  //注册解复用器的函数是av_register_input_format()。
    /*open input file*/  
    AVIOContext *avio_video_in =avio_alloc_context(inbuffer_v, 32768,0,NULL,read_video_buffer,NULL,NULL);    
    if(avio_in==NULL)  
    {
		printf("avio error\r\n");
	}
    ifmt_ctx_v->pb=avio_video_in;   
    ifmt_ctx_v->flags=AVFMT_FLAG_CUSTOM_IO;  

	//ifmt_ctx_v = 
	//ifmt_ctx_v->duration = ifmt_ctx_v->start_time = AV_NOPTS_VALUE;
	if ((ret = avformat_open_input(&ifmt_ctx_v, "whatever", NULL, NULL)) < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
		return ret;
	}

	
	for (unsigned i=0; i<ifmt_ctx_v->nb_streams; i++)
	{
			if (ifmt_ctx_v->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				i_video_stream = ifmt_ctx_v->streams[i];
				break;
			}
	}

    avformat_alloc_output_context2(&ofmt_ctx, NULL, "ts", NULL);
	#if 0
	ofmt_ctx = avformat_alloc_context();
	//av_register_output_format  //注册复用器的函数是
	ofmt_ctx->oformat = ff_mpegts_muxer;
	
	if (ofmt_ctx->oformat->priv_data_size > 0) {
			ofmt_ctx->priv_data = av_mallocz(ofmt_ctx->oformat->priv_data_size);
			if (!ofmt_ctx->priv_data)
				goto nomem;
			if (s->oformat->priv_class) {
				*(const AVClass**)ofmt_ctx->priv_data= ofmt_ctx->oformat->priv_class;
				av_opt_set_defaults(ofmt_ctx->priv_data);
			}
	} else
		ofmt_ctx->priv_data = NULL;
#endif

	AVIOContext *avio_out =avio_alloc_context(outbuffer, 32768,1,NULL,NULL,write_AV_buffer,NULL);    
    if(avio_out==NULL)  
    {
		printf("avio error\r\n");
	} 
	
    //avio_out->write_packet=write_packet;  
    ofmt_ctx->pb=avio_out;   
    ofmt_ctx->flags=AVFMT_FLAG_CUSTOM_IO;  

	
	o_video_stream = avformat_new_stream(ofmt_ctx, NULL);
	   {
		   AVCodecContext *c;
		   c = o_video_stream->codec;
		   c->bit_rate = 400000;
		   c->codec_id = AV_CODEC_ID_H264;//i_video_stream->codec->codec_id;
		   c->codec_type = AVMEDIA_TYPE_VIDEO;//i_video_stream->codec->codec_type;
		   c->time_base.num = 1;//i_video_stream->time_base.num;
		   c->time_base.den = 60;//i_video_stream->time_base.den;fps
		   //fprintf(stderr, "time_base.num = %d time_base.den = %d\n", c->time_base.num, c->time_base.den);
		   c->width = 1280;//i_video_stream->codec->width;
		   c->height = 720;//i_video_stream->codec->height;
		   c->pix_fmt = AV_PIX_FMT_YUV420P;//i_video_stream->codec->pix_fmt;
		   c->max_b_frames = 3;
		   c->gop_size = 250;

		   c->qmin = 10;
	       c->qmax = 51;
		   c->qcompress = i_video_stream->codec->qcompress;
	   }
	 
#if 0	
	if ((ret = avformat_open_input(&ifmt_ctx_v, szVideoInput, 0, 0)) < 0) {
		printf("Could not open input file.");
		goto end;
	}
	if ((ret = avformat_find_stream_info(ifmt_ctx_v, 0)) < 0) {
		printf("Failed to retrieve input stream information");
		goto end;
	}


	if ((ret = avformat_open_input(&ifmt_ctx_a, szAudioInput, 0, 0)) < 0) {
		printf("Could not open input file.");
		goto end;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx_a, 0)) < 0) {
		printf("Failed to retrieve input stream information");
		goto end;
	}
	
	//Output  
	avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, szVideoOutput);
	if (!ofmt_ctx) {
		printf("Could not create output context\n");
		ret = AVERROR_UNKNOWN;
		goto end;
	}
	ofmt = ofmt_ctx->oformat;
	
	for (i = 0; i < ifmt_ctx_v->nb_streams; i++) {
		//Create output AVStream according to input AVStream  
		if (ifmt_ctx_v->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			AVStream *in_stream = ifmt_ctx_v->streams[i];
			AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
			videoindex_v = i;
			if (!out_stream) {
				printf("Failed allocating output stream\n");
				ret = AVERROR_UNKNOWN;
				goto end;
			}
			videoindex_out = out_stream->index;
			//Copy the settings of AVCodecContext  
			if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) {
				printf("Failed to copy context from input to output stream codec context\n");
				goto end;
			}
			out_stream->codec->codec_tag = 0;
			if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
				out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
			break;
		}
	}

	for (i = 0; i < ifmt_ctx_a->nb_streams; i++) {
		//Create output AVStream according to input AVStream  
		if (ifmt_ctx_a->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			AVStream *in_stream = ifmt_ctx_a->streams[i];
			AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);

			audioindex_a = i;
			if (!out_stream) {
				printf("Failed allocating output stream\n");
				ret = AVERROR_UNKNOWN;
				goto end;
			}
			audioindex_out = out_stream->index;
			//Copy the settings of AVCodecContext  
			if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) {
				printf("Failed to copy context from input to output stream codec context\n");
				goto end;
			}
			out_stream->codec->codec_tag = 0;
			if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
				out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
			break;
		}
	}

	printf("==========Output Information==========\n");
	av_dump_format(ofmt_ctx, 0, szVideoOutput, 1);
	printf("======================================\n");
	//Open output file  
	if (!(ofmt->flags & AVFMT_NOFILE)) {
		if (avio_open(&ofmt_ctx->pb, szVideoOutput, AVIO_FLAG_WRITE) < 0) {
			printf("Could not open output file '%s'", szVideoOutput);
			goto end;
		}
	}
	#endif
	//Write file header  
	if (avformat_write_header(ofmt_ctx, 0) < 0) {
		printf("Error occurred when opening output file\n");
	}


	while (1) {
		AVFormatContext *ifmt_ctx;
		int stream_index = 0;
		AVStream *in_stream, *out_stream;

		//Get an AVPacket  
		//if (av_compare_ts(cur_pts_v, ifmt_ctx_v->streams[videoindex_v]->time_base, cur_pts_a, ifmt_ctx_a->streams[audioindex_a]->time_base) <= 0) 
	    {
			ifmt_ctx = ifmt_ctx_v;
			stream_index = videoindex_out;
			//printf("Write video Packet time %lld", cur_pts_v * av_q2d(ifmt_ctx_v->streams[videoindex_v]->time_base));
			if (av_read_frame(ifmt_ctx, tmp_pkt) >= 0) {

				do {
					in_stream = ifmt_ctx->streams[tmp_pkt->stream_index];
					out_stream = ofmt_ctx->streams[stream_index];

					if (tmp_pkt->stream_index == videoindex_v) {
						//FIX：No PTS (Example: Raw H.264)  
						//Simple Write PTS  
						//if (tmp_pkt->pts == AV_NOPTS_VALUE) 
						{
							//Write PTS  
							//AVRational time_base1 = in_stream->time_base;
							//Duration between 2 frames (us)  
							//int64_t calc_duration = (int64_t)AV_TIME_BASE / (int64_t)av_q2d(in_stream->r_frame_rate);//(int64_t)av_q2d(in_stream->r_frame_rate) 帧率
							tmp_pkt->pts = frame_index*(out_stream->time_base.den) / ((out_stream->time_base.num) *  o_video_stream->codec->time_base.den);																						 //Parameters  
							//tmp_pkt->pts = (int64_t)(frame_index*calc_duration) / (int64_t)(av_q2d(time_base1)*AV_TIME_BASE);
							tmp_pkt->dts = tmp_pkt->pts;
							//tmp_pkt->duration = (int64_t)calc_duration / (int64_t)(av_q2d(time_base1)*AV_TIME_BASE);
							frame_index++;
						}

						cur_pts_v = tmp_pkt->pts;
						break;
					}
				} while (av_read_frame(ifmt_ctx, tmp_pkt) >= 0);

			}
			else {
				av_packet_unref(tmp_pkt); // 不要忘记减少引用技术
				break;
			}
		}
		#if 0
		else {
			ifmt_ctx = ifmt_ctx_a;
			stream_index = audioindex_out;
			//printf("Write audio Packet time %lld", cur_pts_a * av_q2d(ifmt_ctx_a->streams[audioindex_a]->time_base));
			if (av_read_frame(ifmt_ctx, tmp_pkt) >= 0) {

				do {
					in_stream = ifmt_ctx->streams[tmp_pkt->stream_index];
					out_stream = ofmt_ctx->streams[stream_index];

					if (tmp_pkt->stream_index == audioindex_a) {

						if (index_pts_a == 1)
						{
							inter_pts_a = tmp_pkt->pts;//帧大小即每帧采样数表示一帧中采样的个数，这是恒定值 1152
						}
						cur_pts_a += inter_pts_a;
						break;
					}
				} while (av_read_frame(ifmt_ctx, tmp_pkt) >= 0);
				index_pts_a++;
				tmp_pkt->pts = cur_pts_a;
				tmp_pkt->dts = tmp_pkt->pts;
				//要编解码 转 AAC   avpackt - avframe - avpackt - avsteam
			}
			else {
				av_packet_unref(tmp_pkt); // 不要忘记减少引用技术
				avformat_close_input(&ifmt_ctx_a);
				if ((ret = avformat_open_input(&ifmt_ctx_a, szAudioInput, 0, 0)) < 0) {
					printf("could not open input file.");
					goto end;
				}
				if ((ret = avformat_find_stream_info(ifmt_ctx_a, 0)) < 0) {
					printf("failed to retrieve input stream information");
					goto end;
				}
				//pre_pts_a = cur_pts_a;
				continue;
				//break;
			}
		}
		#endif
		//FIX:Bitstream Filter

		//Convert PTS/DTS  
		//tmp_pkt->pts = av_rescale_q_rnd(tmp_pkt->pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		//tmp_pkt->dts = av_rescale_q_rnd(tmp_pkt->dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		//tmp_pkt->duration = av_rescale_q(tmp_pkt->duration, in_stream->time_base, out_stream->time_base);
		tmp_pkt->pos = -1;
		tmp_pkt->stream_index = stream_index;

		//printf("Write 1 Packet. size:%5d\tpts:%lld\n", (int64_t)av_q2d(in_stream->r_frame_rate), tmp_pkt->pts);
		//Write  
		if (av_interleaved_write_frame(ofmt_ctx, tmp_pkt) < 0) {
			printf("Error muxing packet\n");
			break;
		}
		//av_free_packet(&pkt);
		av_packet_unref(tmp_pkt); // 不要忘记减少引用技术
	}
	//Write file trailer  

	av_write_trailer(ofmt_ctx);
	return true;
	
	
}



Boolean close_ts_muxer(void)
{
	if (!(ofmt_ctx->flags & AVFMT_NOFILE))
		/* Close the output file. */
		avio_closep(&ofmt_ctx->pb);

	av_packet_free(&tmp_pkt);
	avformat_close_input(&ifmt_ctx_v);
	avformat_close_input(&ifmt_ctx_a);
	/* close output */
	if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
		avio_close(ofmt_ctx->pb);
	avformat_free_context(ofmt_ctx);
	if (ret < 0 && ret != AVERROR_EOF) {
		printf("Error occurred.\n");
		return false;
	}
	return true;
}
