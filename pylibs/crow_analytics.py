"""
This is used for the grabbing of googles analytics through the use of a module on the
main CPP application.
"""

from googleapiclient.discovery import build
from oauth2client.service_account import ServiceAccountCredentials

SCOPES = ['https://www.googleapis.com/auth/analytics.readonly']
KEY_FILE_LOCATION = 'private-keys/Intranet-Dashboard-7b5d56f69551.json'
VIEW_ID = '34493859'

def initialize_analyticsreporting():
    """
    Initializes the analytics authorization and module
    """
    credentials = ServiceAccountCredentials.from_json_keyfile_name(
        KEY_FILE_LOCATION, SCOPES)
    analytics = build('analyticsreporting', 'v4', credentials=credentials)
    return analytics

def get_report(analytics):
    """
    Returns the data of the reports module from Google Analytics
    """
    body = {
        'reportRequests': [
            {
                'viewId': VIEW_ID,
                'dateRanges': [{'startDate': '7daysAgo', 'endDate': 'today'}],
                'metrics': [{'expression': 'ga:sessions'}],
                'dimensions': [{'name': 'ga:country'}]
            }]
        }

    return analytics.reports().batchGet(body = {
        'reportRequests': [
            {
                'viewId': VIEW_ID,
                'dateRanges': [{'startDate': '7daysAgo', 'endDate': 'today'}],
                'metrics': [{'expression': 'ga:sessions'}],
                'dimensions': [{'name': 'ga:country'}]
            }]
        }
            ).execute()

def get_response(response):
    """
    Returns all of the data from Google Analytics to the main module
    """
    for report in response.get('reports', []):
        column_header = report.get('columnHeader', {})
        dimension_headers = column_header.get('dimensions', [])
        metric_headers = column_header.get('metricHeader', {}).get('metricHeaderEntries', [])
        return_value = {
            "dimension": [],
            "header": [],
            "metricheader": [],
            "value": []
        }

        for row in report.get('data', {}).get('rows', []):
            dimensions = row.get('dimensions', [])
            daterangevalues = row.get('metrics', [])

            for header, dimension in zip(dimension_headers, dimensions):
                return_value["dimension"].append(dimension)
                return_value["header"].append(header)

            for i, values in enumerate(daterangevalues):
                for metric_header, value in zip(metric_headers, values.get('values')):
                    return_value["metricheader"].append(metric_header.get('name'))
                    return_value["value"].append(value)
    return return_value

def main(return_val):
    """
    Simply used to run the main module
    """
    analytics = initialize_analyticsreporting()
    response = get_report(analytics)
    data = get_response(response)
    if return_val == 'dimension':
        return data["dimension"]
