from graphviz import Digraph

# 创建流程图
dot = Digraph(comment="用户注册与登录流程图", format="png")
dot.attr(rankdir="TB", size="8")

# 开始/结束
dot.node("start", "开始", shape="oval")
dot.node("end", "结束", shape="oval")

# 初始化
dot.node("init", "初始化数据库", shape="box")

# 注册流程
dot.node("register", "用户选择注册", shape="diamond")
dot.node("input_reg", "输入用户名/密码/确认密码", shape="box")
dot.node("check_reg", "检查密码是否一致及非空", shape="diamond")
dot.node("hash_reg", "哈希密码", shape="box")
dot.node("insert_db", "插入数据库", shape="box")
dot.node("exist_user", "用户名已存在？", shape="diamond")
dot.node("reg_fail", "弹窗提示用户名已存在", shape="box")
dot.node("reg_success", "弹窗提示注册成功", shape="box")

# 登录流程
dot.node("login", "用户选择登录", shape="diamond")
dot.node("input_login", "输入用户名/密码", shape="box")
dot.node("check_login", "检查非空", shape="diamond")
dot.node("hash_login", "哈希密码", shape="box")
dot.node("query_db", "查询数据库", shape="box")
dot.node("exist_acc", "账号存在？", shape="diamond")
dot.node("no_acc", "弹窗提示账号不存在", shape="box")
dot.node("pwd_match", "密码匹配？", shape="diamond")
dot.node("pwd_fail", "弹窗提示密码错误", shape="box")
dot.node("login_success", "弹窗提示登录成功", shape="box")
dot.node("op_page", "进入操作界面", shape="box")

# 连接
dot.edge("start", "init")
dot.edge("init", "register")
dot.edge("init", "login")

# 注册路径
dot.edge("register", "input_reg", label="是")
dot.edge("input_reg", "check_reg")
dot.edge("check_reg", "hash_reg", label="通过")
dot.edge("check_reg", "reg_fail", label="不通过")
dot.edge("hash_reg", "insert_db")
dot.edge("insert_db", "exist_user")
dot.edge("exist_user", "reg_fail", label="是")
dot.edge("exist_user", "reg_success", label="否")
dot.edge("reg_success", "op_page")

# 登录路径
dot.edge("login", "input_login", label="是")
dot.edge("input_login", "check_login")
dot.edge("check_login", "hash_login", label="通过")
dot.edge("check_login", "no_acc", label="不通过")
dot.edge("hash_login", "query_db")
dot.edge("query_db", "exist_acc")
dot.edge("exist_acc", "no_acc", label="否")
dot.edge("exist_acc", "pwd_match", label="是")
dot.edge("pwd_match", "pwd_fail", label="否")
dot.edge("pwd_match", "login_success", label="是")
dot.edge("login_success", "op_page")

# 操作界面到结束
dot.edge("op_page", "end")

# 渲染输出
output_path = "/data111"
dot.render(output_path, cleanup=True)

output_path + ".png"
