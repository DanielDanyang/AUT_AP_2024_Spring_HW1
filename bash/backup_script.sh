#!/bin/bash

# 检查至少有两个参数（源路径和目标路径）
if [ "$#" -lt 2 ]; then
    echo "错误：必须提供源路径和目标路径"
    exit 1
fi

# 获取参数
SOURCE_PATH=$1
DESTINATION_PATH=$2
COMPRESSION_LEVEL=1  # 默认压缩等级为1

# 处理可选的压缩等级参数
while [ "$#" -gt 0 ]; do
    case "$1" in
        -c|--compression)
            COMPRESSION_LEVEL=$2
            shift 2
            ;;
        *)
            shift
            ;;
    esac
done

# 获取当前日期
DATE=$(date +%Y-%m-%d)
DIR_NAME=$(basename "$SOURCE_PATH")
BACKUP_FILE="$DESTINATION_PATH/${DATE}_${DIR_NAME}.zip"

# 使用 zip 创建备份
if ! zip -r -"$COMPRESSION_LEVEL" "$BACKUP_FILE" "$SOURCE_PATH"; then
    echo "备份失败。"
    exit 1
fi

# 获取机器信息
MACHINE_INFO=$(uname -a)

# 获取开始时间
START_TIME=$(date +%T)

# 执行备份
if zip -r -"$COMPRESSION_LEVEL" "$BACKUP_FILE" "$SOURCE_PATH"; then
    # 获取结束时间
    END_TIME=$(date +%T)

    # 统计文件和目录数量
    FILE_COUNT=$(find "$SOURCE_PATH" -type f | wc -l)
    DIR_COUNT=$(find "$SOURCE_PATH" -type d | wc -l)

    # 计算总用时
    START_TIMESTAMP=$(date -d "$START_TIME" +%s)
    END_TIMESTAMP=$(date -d "$END_TIME" +%s)
    DURATION=$((END_TIMESTAMP - START_TIMESTAMP))

    # 写入日志文件
    LOG_FILE="$DESTINATION_PATH/backup_log.txt"
    echo "Backup Log Report" > "$LOG_FILE"
    echo "" >> "$LOG_FILE"
    echo "Date: $DATE" >> "$LOG_FILE"
    echo "Time: $START_TIME UTC" >> "$LOG_FILE"
    echo "Machine Information:" >> "$LOG_FILE"
    echo "$MACHINE_INFO" >> "$LOG_FILE"
    echo "" >> "$LOG_FILE"
    echo "Backup Details:" >> "$LOG_FILE"
    echo "Source Path: $SOURCE_PATH" >> "$LOG_FILE"
    echo "Destination Path: $BACKUP_FILE" >> "$LOG_FILE"
    echo "Compression Level: $COMPRESSION_LEVEL" >> "$LOG_FILE"
    echo "Files Backed Up: $FILE_COUNT" >> "$LOG_FILE"
    echo "Directories Backed Up: $DIR_COUNT" >> "$LOG_FILE"
    echo "" >> "$LOG_FILE"
    echo "Backup Summary:" >> "$LOG_FILE"
    echo "Start Time: $START_TIME UTC" >> "$LOG_FILE"
    echo "End Time: $END_TIME UTC" >> "$LOG_FILE"
    echo "Total Duration: $((DURATION / 60)) minutes $((DURATION % 60)) seconds" >> "$LOG_FILE"
else
    echo "备份失败。"
    exit 1
fi
