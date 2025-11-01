#!/usr/bin/python3
import sys
import json

envs = json.load(sys.stdin)
build_yaml = open(".forgejo/shell/build.yaml", "r").read()

env_names = []
for env in envs:
    env_name: str = env[0]
    if env_name.startswith("env:"):
        env_name = env_name[4::]
    else:
        continue
    env_names.append(env_name)

end_data = build_yaml.replace('"REPLACE WITH TARGETS"', ", ".join(env_names))
open("new_build.yaml", "w").write(end_data)
