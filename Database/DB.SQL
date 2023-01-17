-- t_user Table Create SQL
CREATE TABLE t_user
(
    `user_id`         VARCHAR(20)      NOT NULL    COMMENT '회원 아이디', 
    `user_pw`         VARCHAR(20)      NOT NULL    COMMENT '회원 비밀번호', 
    `user_no`         VARCHAR(20)      NOT NULL    COMMENT '회원 번호', 
    `user_name`       VARCHAR(20)      NOT NULL    COMMENT '회원 이름', 
    `user_weight`     DECIMAL(15,2)    NOT NULL    COMMENT '회원 몸무게', 
    `user_birthyear`  YEAR             NOT NULL    COMMENT '회원 출생년도', 
    `user_phone`      VARCHAR(20)      NOT NULL    COMMENT '회원 핸드폰', 
    `user_joindate`   DATETIME         NOT NULL    DEFAULT now() COMMENT '회원 가입일자', 
    `user_type`       CHAR(1)          NOT NULL    COMMENT '회원 유형', 
     PRIMARY KEY (user_id)
);

ALTER TABLE t_user COMMENT '사용자 테이블';


-- t_reward Table Create SQL
CREATE TABLE t_reward
(
    `nft_seq`  INT UNSIGNED    NOT NULL    AUTO_INCREMENT COMMENT 'NFT 순번', 
    `user_id`  VARCHAR(20)     NOT NULL    COMMENT '회원 아이디', 
    `nft`      LONGTEXT        NOT NULL    COMMENT 'NFT 값', 
     PRIMARY KEY (nft_seq)
);

ALTER TABLE t_reward COMMENT '보상';

ALTER TABLE t_reward
    ADD CONSTRAINT FK_t_reward_user_id_t_user_use FOREIGN KEY (user_id)
        REFERENCES t_user (user_id) ON DELETE RESTRICT ON UPDATE RESTRICT;


-- t_cupholder Table Create SQL
CREATE TABLE t_cupholder
(
    `product_id`  VARCHAR(20)    NOT NULL    COMMENT '제품 아이디', 
    `user_id`     VARCHAR(20)    NOT NULL    COMMENT '사용자 아이디', 
    `av_time`     DATETIME       NOT NULL    DEFAULT now() COMMENT '각도 변화 시간', 
    `x1`          INT            NULL        COMMENT 'X1', 
    `y1`          INT            NULL        COMMENT 'Y1', 
    `x2`          INT            NULL        COMMENT 'X2',
    `y2`          INT            NULL        COMMENT 'y2', 
    `x3`          INT            NULL        COMMENT 'X3', 
    `y3`          INT            NULL        COMMENT 'Y3', 
    `x4`          INT            NULL        COMMENT 'X4', 
    `y4`          INT            NULL        COMMENT 'Y4', 
    `x5`          INT            NULL        COMMENT 'X5', 
    `y5`          INT            NULL        COMMENT 'Y5', 
    `x6`          INT            NULL        COMMENT 'X6', 
    `y6`          INT            NULL        COMMENT 'Y6', 
    `x7`          INT            NULL        COMMENT 'X7', 
    `y7`          INT            NULL        COMMENT 'Y7', 
    `x8`          INT            NULL        COMMENT 'X8', 
    `y8`          INT            NULL        COMMENT 'Y8', 
    `x9`          INT            NULL        COMMENT 'X9', 
    `y9`          INT            NULL        COMMENT 'Y9', 
    `x10`         INT            NULL        COMMENT 'X10', 
    `y10`         INT            NULL        COMMENT 'Y10', 
    `x11`         INT            NULL        COMMENT 'X11', 
    `y11`         INT            NULL        COMMENT 'Y11', 
    `x12`         INT            NULL        COMMENT 'X12', 
    `y12`         INT            NULL        COMMENT 'Y12', 
    `x13`         INT            NULL        COMMENT 'X13', 
    `y13`         INT            NULL        COMMENT 'Y13', 
    `x14`         INT            NULL        COMMENT 'X14', 
    `y14`         INT            NULL        COMMENT 'Y14', 
    `x15`         INT            NULL        COMMENT 'X15', 
    `y15`         INT            NULL        COMMENT 'Y15', 
    `x16`         INT            NULL        COMMENT 'X16', 
    `y16`         INT            NULL        COMMENT 'Y16', 
    `x17`         INT            NULL        COMMENT 'X17', 
    `y17`         INT            NULL        COMMENT 'Y17', 
    `x18`         INT            NULL        COMMENT 'X18', 
    `y18`         INT            NULL        COMMENT 'Y18', 
    `x19`         INT            NULL        COMMENT 'X19', 
    `y19`         INT            NULL        COMMENT 'Y19', 
    `x20`         INT            NULL        COMMENT 'X20', 
    `y20`         INT            NULL        COMMENT 'Y20', 
    `x21`         INT            NULL        COMMENT 'X21', 
    `y21`         INT            NULL        COMMENT 'Y21', 
    `x22`         INT            NULL        COMMENT 'X22', 
    `y22`         INT            NULL        COMMENT 'Y22', 
    `x23`         INT            NULL        COMMENT 'X23', 
    `y23`         INT            NULL        COMMENT 'Y23', 
    `x24`         INT            NULL        COMMENT 'X24', 
    `y24`         INT            NULL        COMMENT 'Y24', 
    `x25`         INT            NULL        COMMENT 'X25', 
    `y25`         INT            NULL        COMMENT 'Y25', 
    `x26`         INT            NULL        COMMENT 'X26', 
    `y26`         INT            NULL        COMMENT 'Y26', 
    `x27`         INT            NULL        COMMENT 'X27', 
    `y27`         INT            NULL        COMMENT 'Y27', 
    `x28`         INT            NULL        COMMENT 'X28', 
    `y28`         INT            NULL        COMMENT 'Y28', 
    `x29`         INT            NULL        COMMENT 'X29', 
    `y29`         INT            NULL        COMMENT 'Y29', 
    `x30`         INT            NULL        COMMENT 'X30', 
    `y30`         INT            NULL        COMMENT 'Y30', 
    `t`           DATETIME       NULL        COMMENT 'T', 
    `label`       INT            NULL        COMMENT 'Label', 
     PRIMARY KEY (product_id)
);

ALTER TABLE t_cupholder COMMENT '컵홀더';

ALTER TABLE t_cupholder
    ADD CONSTRAINT FK_t_cupholder_user_id_t_user_ FOREIGN KEY (user_id)
        REFERENCES t_user (user_id) ON DELETE RESTRICT ON UPDATE RESTRICT;


-- t_coasters Table Create SQL
CREATE TABLE t_coasters
(
    `coaster_seq`  VARCHAR(20)      NOT NULL    COMMENT '컵받침 순번', 
    `user_id`      VARCHAR(20)      NOT NULL    COMMENT '회원 아이디', 
    `wv_time`      DATETIME         NOT NULL    DEFAULT now() COMMENT '무게 변화 시간', 
    `weight`       DECIMAL(15,2)    NOT NULL    COMMENT '무게', 
     PRIMARY KEY (coaster_seq)
);

ALTER TABLE t_coasters COMMENT '컵받침';

ALTER TABLE t_coasters
    ADD CONSTRAINT FK_t_coasters_user_id_t_user_u FOREIGN KEY (user_id)
        REFERENCES t_user (user_id) ON DELETE RESTRICT ON UPDATE RESTRICT;


-- t_achievement Table Create SQL
CREATE TABLE t_achievement
(
    `a_time`           TIMESTAMP       NOT NULL    COMMENT '시간', 
    `user_id`          VARCHAR(20)     NOT NULL    COMMENT '회원 아이디', 
    `1st_achievement`  CHAR(1)         NULL        COMMENT '1일차 달성', 
    `2nd_achievement`  CHAR(1)         NULL        COMMENT '2일차 달성', 
    `reward`           CHAR(1)         NULL        COMMENT '보상 획득 여부', 
    `nft_index`        INT UNSIGNED    NOT NULL    COMMENT 'NFT순번', 
     PRIMARY KEY (a_time)
);

ALTER TABLE t_achievement COMMENT '목표달성';

ALTER TABLE t_achievement
    ADD CONSTRAINT FK_t_achievement_user_id_t_use FOREIGN KEY (user_id)
        REFERENCES t_user (user_id) ON DELETE RESTRICT ON UPDATE RESTRICT;

ALTER TABLE t_achievement
    ADD CONSTRAINT FK_t_achievement_nft_index_t_r FOREIGN KEY (nft_index)
        REFERENCES t_reward (nft_seq) ON DELETE RESTRICT ON UPDATE RESTRICT;


-- t_reward_backup Table Create SQL
CREATE TABLE t_reward_backup
(
    `back_time`  TIMESTAMP       NOT NULL    DEFAULT now() COMMENT '백업시간', 
    `nft_seq`    INT UNSIGNED    NOT NULL    COMMENT 'NFT 순번', 
    `nft`        LONGTEXT        NOT NULL    COMMENT 'NFT 값', 
    `user_id`    VARCHAR(20)     NOT NULL    COMMENT '회원 아이디', 
     PRIMARY KEY (back_time)
);

ALTER TABLE t_reward_backup COMMENT '보상 테이블 백업';