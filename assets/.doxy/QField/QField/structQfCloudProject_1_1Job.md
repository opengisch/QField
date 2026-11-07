

# Struct QfCloudProject::Job



[**ClassList**](annotated.md) **>** [**Job**](structQfCloudProject_1_1Job.md)



_Tracks the job status (status, error etc) for a particular project. For now 1 project can have only 1 job of a type._ 






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  QString | [**id**](#variable-id)  <br> |
|  QString | [**projectId**](#variable-projectid)  <br> |
|  [**QfCloudProject::JobStatus**](classQfCloudProject.md#enum-jobstatus) | [**status**](#variable-status)   = `QfCloudProject::JobPendingStatus`<br> |
|  [**QfCloudProject::JobType**](classQfCloudProject.md#enum-jobtype) | [**type**](#variable-type)   = `QfCloudProject::JobType::Package`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Job**](#function-job-12) (const QString & id, const QString & projectId, const [**QfCloudProject::JobType**](classQfCloudProject.md#enum-jobtype) type) <br> |
|   | [**Job**](#function-job-22) () = default<br> |




























## Public Attributes Documentation




### variable id 

```C++
QString QfCloudProject::Job::id;
```




<hr>



### variable projectId 

```C++
QString QfCloudProject::Job::projectId;
```




<hr>



### variable status 

```C++
QfCloudProject::JobStatus QfCloudProject::Job::status;
```




<hr>



### variable type 

```C++
QfCloudProject::JobType QfCloudProject::Job::type;
```




<hr>
## Public Functions Documentation




### function Job [1/2]

```C++
inline Job::Job (
    const QString & id,
    const QString & projectId,
    const QfCloudProject::JobType type
) 
```




<hr>



### function Job [2/2]

```C++
Job::Job () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfcloudproject.h`

